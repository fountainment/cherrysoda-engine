#include "SpriteBench.h"

#include <CherrySoda/CherrySoda.h>

using spritebench::SpriteBench;

using namespace cherrysoda;

class SpriteRotate : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(SpriteRotate, Component);

	SpriteRotate(float speed)
		: base(true, false)
		, m_speed(speed)
		{}

	void Update() override
	{
		GetEntity()->Get<Sprite>()->RotateOnZ(Engine::Instance()->DeltaTime() * m_speed);
	}

private:
	float m_speed;
};

SpriteBench::SpriteBench()
	: base(500, 500, "SpriteBench")
{
	SetClearColor(Color::Black);
}

void SpriteBench::Update()
{
	base::Update();

	Commands::ClearLog();
	Commands::Log(StringUtil::Format("FPS: %d\n", Engine::Instance()->FPS()));
	// Add global GUI or other global stuffs here
}

void SpriteBench::Initialize()
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

	auto atlas = Atlas::FromAtlas("assets/atlases/cherrysoda.json");
	for (int i = 0; i < 50000; ++i) {
		auto entity = new Entity;
		auto sprite = new Sprite(atlas);
		sprite->Justify(Math::Vec2(0.5f));
		sprite->AddLoop("test", "cherrysoda");
		sprite->Play("test", false, true);
		sprite->Scale2D(Math::Vec2(Calc::GetRandom()->NextFloat(0.1f, 1.5f)));
		entity->Add(sprite);
		entity->Add(new SpriteRotate(Calc::GetRandom()->NextFloat(-Math::Pi2, Math::Pi2)));
		scene->Add(entity);
		entity->Position2D(Math::Vec2(Calc::GetRandom()->NextInt(-200, 200), Calc::GetRandom()->NextInt(-200, 200)));
	}

	SetScene(scene);
}

void SpriteBench::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

void SpriteBench::UnloadContent()
{
	// Unload resources here

	base::UnloadContent();
}
