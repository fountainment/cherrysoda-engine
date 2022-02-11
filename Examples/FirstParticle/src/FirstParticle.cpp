#include "FirstParticle.h"

#include <CherrySoda/CherrySoda.h>

using firstparticle::FirstParticle;

using namespace cherrysoda;

FirstParticle::FirstParticle()
	: base()
{
	SetTitle("FirstParticle");
	SetClearColor(Color::Black);
}

void FirstParticle::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void FirstParticle::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new Scene();

	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 200.f));
	camera->CenterOrigin();

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	auto entity = new Entity();
	entity->PositionY(-50.f);

	scene->Add(renderer);
	scene->Add(entity);

	{
		// Generated from ParticleEditor
		auto particleSystem = new ParticleSystem(-1, 1000);
		auto particleType = new ParticleType();
		particleType->m_color = Color(0.000f,0.765f,1.000f,1.000f);
		particleType->m_color2 = Color(0.189f,0.543f,0.990f,1.000f);
		particleType->m_colorMode = ParticleType::ColorModes::Choose;
		particleType->m_fadeMode = ParticleType::FadeModes::Late;
		particleType->m_speedMin = 325.792f;
		particleType->m_speedMax = 500.000f;
		particleType->m_speedMultiplier = 0.995f;
		particleType->m_acceleration = Math::Vec2(0.000f,-500.000f);
		particleType->m_direction = 1.571f;
		particleType->m_directionRange = 0.512f;
		particleType->m_lifeMin = 0.769f;
		particleType->m_lifeMax = 1.946f;
		particleType->m_spinMin = 11.514f;
		particleType->m_spinMax = 18.196f;
		particleType->m_spinFlippedChance = true;
		particleType->m_rotationMode = ParticleType::RotationModes::Random;
		particleType->m_scaleOut = true;
		auto particleEmitter = new ParticleEmitter(particleSystem, particleType, Vec2_Zero, Math::Vec2(0.000f, 0.000f), 20, 0.017f);
		// Generated from ParticleEditor

		scene->Add(particleSystem);
		entity->Add(particleEmitter);
	}

	SetScene(scene);
}

void FirstParticle::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

