#include "ParticleEditor.h"

#include <CherrySoda/CherrySoda.h>

#include "VonwaonBitmapFont.inl"

using particleeditor::ParticleEditor;

using namespace cherrysoda;

static ParticleSystem* s_particleSystem = nullptr;
static ParticleType* s_particleType = nullptr;
static ParticleEmitter* s_particleEmitter = nullptr;
static Camera* s_camera = nullptr;

class OriginGraphicsCompoent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(OriginGraphicsCompoent, GraphicsComponent);

	OriginGraphicsCompoent() : base(false) {}

	void Render() override
	{
		Draw::Line(Math::Vec2(RenderPosition()), Math::Vec2(RenderPosition()) + Math::Vec2(0.f, 10.f));
		Draw::Line(Math::Vec2(RenderPosition()), Math::Vec2(RenderPosition()) + Math::Vec2(10.f, 0.f));
	}
};

ParticleEditor::ParticleEditor()
	: base(1280, 800, "Particle Editor (Work In Progress) - CherrySoda Engine")
{
	SetClearColor(Color("#606060"));
}

void ParticleEditor::Update()
{
	base::Update();

	float menuHeight = 0.f;
	static Color s_backgroundColor = GetClearColor();

	static int lang_i = 1;
	#define LANGS(A,B) (lang_i?(B):(A))

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu(LANGS(u8"文件", "File"))) {
			if (ImGui::Button(LANGS(u8"导出到剪贴板", "Export to Clipboard"))) {

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(LANGS(u8"选项", "Options"))) {
			if (ImGui::BeginMenu(LANGS(u8"背景颜色", "Background Color"))) {
				ImGuiColorEditFlags colorEditFlags = 0
					| ImGuiColorEditFlags_NoLabel
					| ImGuiColorEditFlags_NoSidePreview
					| ImGuiColorEditFlags_NoSmallPreview
					;
				if (ImGui::ColorPicker3("Color", reinterpret_cast<float*>(&s_backgroundColor), colorEditFlags)) {
					SetClearColor(s_backgroundColor);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu(LANGS(u8"语言", "Language"))) {
				ImGui::Combo(LANGS(u8"语言", "Language"), &lang_i, "中文\0English\0");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		menuHeight = ImGui::GetWindowSize().y;
		ImGui::EndMainMenuBar();
	}

	ImGuiWindowFlags windowFlags = 0
		| ImGuiWindowFlags_NoResize
		| ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoSavedSettings
		| ImGuiWindowFlags_NoDocking
		| ImGuiWindowFlags_NoTitleBar
		;

	ImGui::Begin("Particle Editor", nullptr, windowFlags);
	{
		ImGui::SetWindowPos(ImVec2(GetWindowWidth() - 375.f, menuHeight));
		ImGui::SetWindowSize(ImVec2(375.f, GetWindowHeight() - menuHeight));

		ImGui::BeginChild("Emitter", ImVec2(0.f, 115.f), true);
		ImGui::Text(LANGS(u8"发射器参数:", "Emitter Parameters:"));
		ImGui::Indent();
		{
			static float s_interval = s_particleEmitter->Interval();
			static int s_amount = s_particleEmitter->Amount();
			static Math::Vec2 s_positionRange = s_particleEmitter->PositionRange();
			if (ImGui::SliderFloat(LANGS(u8"发射间隔", "Interval"), &s_interval, 1.f / 60.f, 5.f)) {
				s_particleEmitter->Interval(s_interval);
			}
			if (ImGui::DragInt(LANGS(u8"发射数量", "Amount"), &s_amount, 1, 1, 1000)) {
				s_particleEmitter->Amount(s_amount);
			}
			if (ImGui::DragFloat2(LANGS(u8"位置范围", "Position Range"), &s_positionRange.x, 1.0f, 0.f, 1000.f)) {
				s_particleEmitter->PositionRange(s_positionRange);
			}
		}
		int activeAmount = s_particleSystem->ActiveAmount();
		ImGui::PushItemWidth(160.f);
		ImGui::LabelText("##ActiveAmount", LANGS(u8"当前总数:%4d", "Amount:%4d"), activeAmount); ImGui::SameLine();
		ImGui::PopItemWidth();
		static int s_maxActiveAmount = activeAmount;
		s_maxActiveAmount = Math_Max(s_maxActiveAmount, activeAmount);
		ImGui::PushItemWidth(70.f);
		ImGui::LabelText("##MaxActiveAmount", LANGS(u8"最大:%4d", "Max:%4d"), s_maxActiveAmount); ImGui::SameLine();
		ImGui::PopItemWidth();
		if (ImGui::Button(LANGS(u8"重置最大统计", "Clear Max"))) {
			s_maxActiveAmount = 0;
		}
		ImGui::EndChild();

		ImGui::BeginChild("Type", ImVec2(0.f, 0.f), true);
		ImGui::Text(LANGS(u8"颜色:", "Colors:"));
		ImGui::Indent();
		{
			ImGui::ColorEdit4(LANGS(u8"颜色", "Color"), reinterpret_cast<float*>(&s_particleType->m_color), ImGuiColorEditFlags_AlphaPreview);
			ImGui::ColorEdit4(LANGS(u8"颜色2", "Color2"), reinterpret_cast<float*>(&s_particleType->m_color2), ImGuiColorEditFlags_AlphaPreview);
			static int s_colorMode = static_cast<int>(s_particleType->m_colorMode);
			if (ImGui::Combo(LANGS(u8"颜色模式", "Color Mode"), &s_colorMode, LANGS(u8"静态\0选择\0闪烁\0褪色\0", "Static\0Choose\0Blink\0Fade\0"))) {
				s_particleType->m_colorMode = static_cast<ParticleType::ColorModes>(s_colorMode);
			}
			static int s_fadeMode = 0;
			if (ImGui::Combo(LANGS(u8"褪色模式", "Fade Mode"), &s_fadeMode, LANGS(u8"无\0线性\0延迟\0缓动进出\0", "None\0Linear\0Late\0InAndOut\0"))) {
				s_particleType->m_fadeMode = static_cast<ParticleType::FadeModes>(s_fadeMode);
			}
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"速度:", "Speed:"));
		ImGui::Indent();
		{
			ImGui::SliderFloat(LANGS(u8"最小速度", "Speed Min"), &s_particleType->m_speedMin, 0.f, 500.f);
			ImGui::SliderFloat(LANGS(u8"最大速度", "Speed Max"), &s_particleType->m_speedMax, 0.f, 500.f);
			ImGui::SliderFloat(LANGS(u8"乘数", "Multiplier"), &s_particleType->m_speedMultiplier, 0.f, 10.f);
			ImGui::DragFloat2(LANGS(u8"加速度", "Acceleration"), &s_particleType->m_acceleration.x, 1.f, -500.f, 500.f);
			ImGui::SliderFloat(LANGS(u8"摩擦力", "Friction"), &s_particleType->m_friction, 0.f, 500.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"方向:", "Direction:"));
		ImGui::Indent();
		{
			ImGui::SliderFloat(LANGS(u8"方向", "Direction"), &s_particleType->m_direction, 0.f, Math::Pi2);
			ImGui::SliderFloat(LANGS(u8"方向范围", "Range##Direction"), &s_particleType->m_directionRange, 0.f, Math::Pi2);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"生命周期:", "Life:"));
		ImGui::Indent();
		{
			ImGui::SliderFloat(LANGS(u8"最小生命周期", "Life Min"), &s_particleType->m_lifeMin, 0.f, 10.f);
			ImGui::SliderFloat(LANGS(u8"最大生命周期", "Life Max"), &s_particleType->m_lifeMax, 0.f, 10.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"大小:", "Size:"));
		ImGui::Indent();
		{
			ImGui::DragFloat(LANGS(u8"大小", "Size"), &s_particleType->m_size, 0.1f, 0.f, 10.f);
			ImGui::DragFloat(LANGS(u8"大小范围", "Range##Size"), &s_particleType->m_sizeRange, 0.1f, 0.f, 10.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"转向:", "Rotation:"));
		ImGui::Indent();
		{
			ImGui::SliderFloat(LANGS(u8"最小旋转速度", "Spin Min"), &s_particleType->m_spinMin, 0.f, Math::Pi2 * 5.f);
			ImGui::SliderFloat(LANGS(u8"最大旋转速度", "Spin Max"), &s_particleType->m_spinMax, 0.f, Math::Pi2 * 5.f);
			ImGui::Checkbox(LANGS(u8"一定概率反向旋转", "Spin Flipped Chance"), &s_particleType->m_spinFlippedChance);
			static int s_rotationMode = static_cast<int>(s_particleType->m_rotationMode);
			ImGui::Combo(LANGS(u8"转向模式", "Rotation Mode"), &s_rotationMode, LANGS(u8"无\0随机\0与运动方向相同\0", "None\0Random\0SameAsDirection\0"));
			s_particleType->m_rotationMode = static_cast<ParticleType::RotationModes>(s_rotationMode);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text(LANGS(u8"其他:", "Other:"));
		ImGui::Indent();
		{
			ImGui::Checkbox(LANGS(u8"缩放渐隐", "Scale Out"), &s_particleType->m_scaleOut);
			ImGui::Checkbox(LANGS(u8"使用实际增量时间", "Use Actual Delta Time"), &s_particleType->m_useActualDeltaTime);
		}
		ImGui::Unindent();
		ImGui::EndChild();
	}
	ImGui::End();

	if (MInput::Mouse()->CheckRightButton()) {
		s_camera->MovePosition2D(-MInput::Mouse()->PositionDelta());
	}
}

void ParticleEditor::Initialize()
{
	base::Initialize();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->Clear();
	io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontFromMemoryCompressedTTF(vowaon_compressed_data, vowaon_compressed_size, 13.f, &config, io.Fonts->GetGlyphRangesChineseFull());
	GUI::BuildFontTexture();

	auto scene = new Scene();
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 100.f));
	camera->CenterOrigin();
	s_camera = camera;

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	scene->Add(renderer);

	s_particleSystem = new ParticleSystem(-1, 1000);
	s_particleType = new ParticleType();
	scene->Add(s_particleSystem);

	s_particleType->m_speedMin = 0.f;
	s_particleType->m_speedMax = 100.f;
	s_particleType->m_lifeMin = 0.f;
	s_particleType->m_lifeMax = 1.f;
	s_particleType->m_directionRange = Math::Pi2;
	auto entity = new Entity();
	s_particleEmitter = new ParticleEmitter(s_particleSystem, s_particleType, Vec2_Zero, Vec2_Zero, 100, 1.0f);
	entity->Add(new OriginGraphicsCompoent);
	entity->Add(s_particleEmitter);

	scene->Add(entity);

	SetScene(scene);
}

void ParticleEditor::LoadContent()
{
	base::LoadContent();
}

