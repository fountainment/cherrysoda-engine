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

	auto entity = new Entity();
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 200.f));
	camera->CenterOrigin();

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	scene->Add(entity);
	scene->Add(renderer);

	auto pSys = new ParticleSystem(-1, 20000);
	auto pType = new ParticleType();
	pType->m_color = Color::Yellow;
	pType->m_color2 = Color::Transparent;
	pType->m_speedMin = 0.f;
	pType->m_speedMax = 200.f;
	pType->m_sizeRange = .5f;
	pType->m_lifeMin = 1.f;
	pType->m_lifeMax = 2.f;
	pType->m_directionRange = Math::Pi2;
	pType->m_colorMode = ParticleType::ColorModes::Fade;
	pType->m_rotationMode = ParticleType::RotationModes::Random;
	pType->m_acceleration = Math::Vec2(0.f, -100.f);
	pType->m_scaleOut = true;

	auto pEmitter = new ParticleEmitter(pSys, pType, Math::Vec2(0.f, 50.f), Vec2_Zero, 500, 1.0f);
	entity->Add(pEmitter);
	scene->Add(pSys);

	SetScene(scene);
}

void FirstParticle::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

