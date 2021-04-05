#include "FirstSprite.h"

#include <CherrySoda/CherrySoda.h>

using example::FirstSprite;

using namespace cherrysoda;

FirstSprite::FirstSprite() : base()
{
	SetTitle("FirstSprite");
	SetClearColor(Color::Black);
}

void FirstSprite::Initialize()
{
	base::Initialize();

	auto scene = new Scene();
	auto entity = new Entity();

	Graphics::SetPointTextureSampling();

	auto sprite = new Sprite("assets/atlases/cherry_soda.json");
	sprite->AddLoop("showcase", "cherrysoda");
	sprite->Play("showcase");
	auto renderer = new EverythingRenderer();

	entity->Add(sprite);
	scene->Add(entity);

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 200.f));
	camera->Scale2D(Math::IVec2(2.0f));
	camera->CenterOrigin();
	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	scene->Add(renderer);

	SetScene(scene);
}
