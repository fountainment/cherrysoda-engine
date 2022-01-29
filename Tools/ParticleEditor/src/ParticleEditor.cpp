#include "ParticleEditor.h"

#include <CherrySoda/CherrySoda.h>

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
	SetClearColor(Color::Black);
}

void ParticleEditor::Update()
{
	base::Update();

	ImGuiWindowFlags windowFlags = 0 |
	                               ImGuiWindowFlags_NoResize |
	                               ImGuiWindowFlags_NoMove |
	                               ImGuiWindowFlags_NoSavedSettings |
	                               ImGuiWindowFlags_NoDocking |
	                               ImGuiWindowFlags_MenuBar;
	ImGui::Begin("Particle Editor", nullptr, windowFlags);
	{
		ImGui::SetWindowPos(ImVec2(0.f, 0.f));
		ImGui::SetWindowSize(ImVec2(375.f, Engine::Instance()->GetWindowHeight()));

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				// TODO: Add save and load option
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::BeginChild("Emitter", ImVec2(0.f, 115.f), true);
		ImGui::Text("Emitter Parameters:");
		ImGui::Indent();
		{
			static float s_interval = s_particleEmitter->Interval();
			static int s_amount = s_particleEmitter->Amount();
			static Math::Vec2 s_positionRange = s_particleEmitter->PositionRange();
			if (ImGui::SliderFloat("Interval", &s_interval, 1.f / 60.f, 5.f)) {
				s_particleEmitter->Interval(s_interval);
			}
			if (ImGui::DragInt("Amount", &s_amount, 1, 1, 1000)) {
				s_particleEmitter->Amount(s_amount);
			}
			if (ImGui::DragFloat2("Position Range", &s_positionRange.x, 1.0f, 0.f, 1000.f)) {
				s_particleEmitter->PositionRange(s_positionRange);
			}
		}
		int activeAmount = s_particleSystem->ActiveAmount();
		ImGui::PushItemWidth(160.f);
		ImGui::LabelText("##ActiveAmount", "Amount:%4d", activeAmount); ImGui::SameLine();
		ImGui::PopItemWidth();
		static int s_maxActiveAmount = activeAmount;
		s_maxActiveAmount = Math_Max(s_maxActiveAmount, activeAmount);
		ImGui::PushItemWidth(70.f);
		ImGui::LabelText("##MaxActiveAmount", "Max:%4d", s_maxActiveAmount); ImGui::SameLine();
		ImGui::PopItemWidth();
		if (ImGui::Button("Clear Max")) {
			s_maxActiveAmount = 0;
		}
		ImGui::EndChild();

				ImGui::BeginChild("Type", ImVec2(0.f, 0.f), true);
		ImGui::Text("Colors:");
		ImGui::Indent();
		{
			ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&s_particleType->m_color), ImGuiColorEditFlags_AlphaPreview);
			ImGui::ColorEdit4("Color2", reinterpret_cast<float*>(&s_particleType->m_color2), ImGuiColorEditFlags_AlphaPreview);
			static int s_colorMode = static_cast<int>(s_particleType->m_colorMode);
			if (ImGui::Combo("Color Mode", &s_colorMode, "Static\0Choose\0Blink\0Fade\0")) {
				s_particleType->m_colorMode = static_cast<ParticleType::ColorModes>(s_colorMode);
			}
			static int s_fadeMode = 0;
			if (ImGui::Combo("Fade Mode", &s_fadeMode, "None\0Linear\0Late\0InAndOut\0")) {
				s_particleType->m_fadeMode = static_cast<ParticleType::FadeModes>(s_fadeMode);
			}
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Speed:");
		ImGui::Indent();
		{
			ImGui::SliderFloat("Speed Min", &s_particleType->m_speedMin, 0.f, 500.f);
			ImGui::SliderFloat("Speed Max", &s_particleType->m_speedMax, 0.f, 500.f);
			ImGui::SliderFloat("Multiplier", &s_particleType->m_speedMultiplier, 0.f, 10.f);
			ImGui::DragFloat2("Acceleration", &s_particleType->m_acceleration.x, 1.f, -500.f, 500.f);
			ImGui::SliderFloat("Friction", &s_particleType->m_friction, 0.f, 500.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Direction:");
		ImGui::Indent();
		{
			ImGui::SliderFloat("Direction", &s_particleType->m_direction, 0.f, Math::Pi2);
			ImGui::SliderFloat("Range##Direction", &s_particleType->m_directionRange, 0.f, Math::Pi2);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Life:");
		ImGui::Indent();
		{
			ImGui::SliderFloat("Life Min", &s_particleType->m_lifeMin, 0.f, 10.f);
			ImGui::SliderFloat("Life Max", &s_particleType->m_lifeMax, 0.f, 10.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Size:");
		ImGui::Indent();
		{
			ImGui::DragFloat("Size", &s_particleType->m_size, 0.1f, 0.f, 10.f);
			ImGui::DragFloat("Range##Size", &s_particleType->m_sizeRange, 0.1f, 0.f, 10.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Rotation:");
		ImGui::Indent();
		{
			ImGui::SliderFloat("Spin Min", &s_particleType->m_spinMin, 0.f, Math::Pi2 * 5.f);
			ImGui::SliderFloat("Spin Max", &s_particleType->m_spinMax, 0.f, Math::Pi2 * 5.f);
			ImGui::Checkbox("Spin Flipped Chance", &s_particleType->m_spinFlippedChance);
			static int s_rotationMode = static_cast<int>(s_particleType->m_rotationMode);
			ImGui::Combo("Rotation Mode", &s_rotationMode, "None\0Random\0SameAsDirection\0");	
			s_particleType->m_rotationMode = static_cast<ParticleType::RotationModes>(s_rotationMode);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Other:");
		ImGui::Indent();
		{
			ImGui::Checkbox("Scale Out", &s_particleType->m_scaleOut);
			ImGui::Checkbox("Use Actual Delta Time", &s_particleType->m_useActualDeltaTime);
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

