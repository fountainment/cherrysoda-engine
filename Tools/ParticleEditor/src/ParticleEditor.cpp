#include "ParticleEditor.h"

#include <CherrySoda/CherrySoda.h>

using particleeditor::ParticleEditor;

using namespace cherrysoda;

static ParticleType* s_particleType;
static ParticleEmitter* s_particleEmitter;

ParticleEditor::ParticleEditor()
	: base(1280, 800, "Particle Editor (Work In Progress)")
{
	SetClearColor(Color::Black);
}

void ParticleEditor::Update()
{
	base::Update();

	ImGui::Begin("Particle Editor");
	{
		ImGui::BeginChild("Type", ImVec2(0.f, 645.f), true);
		ImGui::Text("Colors:");
		ImGui::Indent();
		{
			ImGui::ColorEdit4("Color", reinterpret_cast<float*>(&s_particleType->m_color));
			ImGui::ColorEdit4("Color2", reinterpret_cast<float*>(&s_particleType->m_color2));
			static int s_colorMode = 0;
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
			ImGui::SliderFloat("Size", &s_particleType->m_size, 0.f, 5.f);
			ImGui::SliderFloat("Range##Size", &s_particleType->m_sizeRange, 0.f, 5.f);
		}
		ImGui::Unindent();
		ImGui::Separator();

		ImGui::Text("Rotation:");
		ImGui::Indent();
		{
			ImGui::SliderFloat("Spin Min", &s_particleType->m_spinMin, 0.f, Math::Pi2 * 100.f);
			ImGui::SliderFloat("Spin Max", &s_particleType->m_spinMax, 0.f, Math::Pi2 * 100.f);
			ImGui::Checkbox("Spin Flipped Chance", &s_particleType->m_spinFlippedChance);
			static int s_rotationMode = 0;
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

		ImGui::BeginChild("Emitter", ImVec2(0.f, 0.f), true);
		ImGui::Text("Emitter Parameters:");
		ImGui::Indent();
		{
			static float s_interval = 0.5f;
			static int s_amount = 500;
			static Math::Vec2 s_positionRange = Vec2_Zero;
			if (ImGui::SliderFloat("Interval", &s_interval, 0.016f, 5.f)) {
			}
			if (ImGui::SliderInt("Amount", &s_amount, 1, 1000)) {

			}
			if (ImGui::SliderFloat2("Position Range", &s_positionRange.x, -100.f, 100.f)) {

			}
		}
		ImGui::EndChild();
	}
	ImGui::End();

	// ImGui::ShowDemoWindow();

	// Add global GUI or other global stuffs here
}

void ParticleEditor::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new Scene();
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 100.f));
	camera->CenterOrigin();

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	scene->Add(renderer);

	auto particleSystem = new ParticleSystem(-1, 1000);
	s_particleType = new ParticleType();
	scene->Add(particleSystem);

	s_particleType->m_speedMin = 10.f;
	s_particleType->m_speedMax = 100.f;
	s_particleType->m_lifeMin = 0.5f;
	s_particleType->m_lifeMax = 1.f;
	s_particleType->m_directionRange = Math::Pi2;
	auto entity = new Entity();
	s_particleEmitter = new ParticleEmitter(particleSystem, s_particleType, Vec2_Zero, Vec2_Zero, 500, 1.0f);
	entity->Add(s_particleEmitter);

	scene->Add(entity);

	SetScene(scene);
}

void ParticleEditor::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

