#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"

using namespace cherrysoda;
using ld42_bulletjam::BulletJamScene;

void BulletJamScene::Begin()
{
	// Renderer
	auto renderer = new EverythingRenderer();
	Add(renderer);
	// Renderer effect
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	// Renderer camera
	auto halfViewSize = Math::Vec2(Engine::Instance()->GetViewSize()) * 0.5f;
	auto camera = renderer->GetCamera();
	camera->Position(Math::Vec3(halfViewSize, halfViewSize.y));
	camera->FOV(90.f);


	// Entities
	auto atlas = GameApp::GetAtlas();
	// Background
	auto imgBG = new Image(atlas->GetAtlasSubtextureFromAtlasAt("BG"));
	auto entBG = new Entity();
	entBG->Add(imgBG);
	Add(entBG);
	// UI for game opening
	auto openUI = new Entity();
	auto openUIImage = new Image(atlas->GetAtlasSubtextureFromAtlasAt("openUI"));
	float openUIHeight = openUIImage->Height();
	openUI->Add(openUIImage);
	m_openUITween = Tween::Create(TweenMode::Oneshot, Ease::QuadInOut, 0.5f);
	m_openUITween->OnUpdate([openUI, openUIHeight](Tween* t){ openUI->PositionY(t->Eased() * openUIHeight); });
	openUI->Add(m_openUITween);
	Add(openUI);

	base::Begin();
}

void BulletJamScene::Update()
{
	// Add scene update here
	if (MInput::Keyboard()->Pressed(Keys::Space))
	{
		Restart();
		Start();
	}

	base::Update();
}

void BulletJamScene::Restart()
{

}

void BulletJamScene::Start()
{
	if (!m_inGameProgress) {
		m_inGameProgress = true;
		m_openUITween->Start();
	}
}
