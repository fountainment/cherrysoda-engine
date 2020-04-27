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

	auto sprite = new Sprite("assets/atlases/cherry_soda.json");
	sprite->AddLoop("cherrysoda", "cherry_soda");
	sprite->Play("cherrysoda");
	auto renderer = new EverythingRenderer();

	entity->Add(sprite);
	scene->Add(entity);
	
	renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 200.f));
	renderer->SetEffect(Effect::LoadEffect("sprite"));
	scene->Add(renderer);

	SetScene(scene);
}
