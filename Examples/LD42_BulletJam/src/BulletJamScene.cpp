#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"

using namespace cherrysoda;
using ld42_bulletjam::BulletJamScene;

void BulletJamScene::Begin()
{
	// Renderer
	auto renderer = new EverythingRenderer();
	// Renderer effect
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	// Remderer camera
	auto halfViewSize = Math::Vec2(Engine::Instance()->GetViewSize()) * 0.5f;
	auto camera = renderer->GetCamera();
	camera->Position(Math::Vec3(halfViewSize, halfViewSize.y));
	camera->FOV(90.f);
	Add(renderer);

	// Entities
	auto atlas = GameApp::GetAtlas();
	auto imgBG = new Image(atlas->GetAtlasSubtextureFromAtlasAt("BG"));
	auto entBG = new Entity();

	entBG->Add(imgBG);

	Add(entBG);

	base::Begin();
}

void BulletJamScene::Update()
{
	// Add scene update here

	base::Update();
}
