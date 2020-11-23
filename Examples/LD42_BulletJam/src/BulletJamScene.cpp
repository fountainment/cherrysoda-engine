#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"
#include "Player.h"

using namespace cherrysoda;
using namespace ld42_bulletjam;

void BulletJamScene::Begin()
{
	// Renderer
	m_renderer = new EverythingRenderer();
	Add(m_renderer);
	// Renderer effect
	m_renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	// Renderer camera
	auto halfViewSize = Math::Vec2(Engine::Instance()->GetViewSize()) * 0.5f;
	auto camera = m_renderer->GetCamera();
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
	openUI->Depth(-999999);
	Add(openUI);
	// Player HP UI
	auto hpUI = new Entity();
	m_playerHPImage = new Image(atlas->GetAtlasSubtextureFromAtlasAt("player_HP/player_HP1"));
	hpUI->Position(Math::Vec2(50.f, 620.f - m_playerHPImage->Height()));
	hpUI->Add(m_playerHPImage);
	Add(hpUI);
	// Player Bomb UI
	auto bombUI = new Entity();
	m_playerBombImage = new Image(atlas->GetAtlasSubtextureFromAtlasAt("bomb_num/bomb_num1"));
	bombUI->Position(Math::Vec2(400.f, 620.f - m_playerBombImage->Height()));
	bombUI->Add(m_playerBombImage);
	Add(bombUI);

	// Alarm
	m_enemyTimer = Alarm::Create(AlarmMode::Persist, [this]()
		{
			static int Wave = 0;
			GenerateEnemy(Wave);
			Wave++;
		}, 3.f);
	Player::Instance()->Add(m_enemyTimer);

	base::Begin();
}

void BulletJamScene::Update()
{
	// Add scene update here
	if (MInput::Keyboard()->Pressed(Keys::Space))
	{
		GameRestart();
		GameStart();
	}

	m_renderer->GetCamera()->SetSize(Engine::Instance()->GetViewSize());

	base::Update();
}

void BulletJamScene::GameRestart()
{
}

void BulletJamScene::GameStart()
{
	if (!m_inGameProgress) {
		m_inGameProgress = true;

		Add(Player::Instance());
		m_enemyTimer->Start();	
		m_openUITween->Start();
	}
}

void BulletJamScene::GameOver()
{
	if (!m_gameHasEnded)
	{
		Tween* tween = Tween::Create(TweenMode::Oneshot, Ease::QuadInOut, 0.5f, true);
		Entity* entity = new Entity();
		Image* loseImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("loseUI"));
		loseImage->SetColorA(0.f);
		entity->Add(loseImage);
		entity->Add(tween);
		tween->OnUpdate([loseImage](Tween* t){ loseImage->SetColorA(t->Eased()); });
		entity->Depth(-999999);
		Add(entity);
		m_gameHasEnded = true;
	}
}

void BulletJamScene::Win()
{
	if (!m_gameHasEnded)
	{
		Tween* tween = Tween::Create(TweenMode::Oneshot, Ease::QuadInOut, 0.5f, true);
		Entity* entity = new Entity();
		Image* winImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("winUI"));
		winImage->SetColorA(0);
		entity->Add(winImage);
		entity->Add(tween);
		tween->OnUpdate([winImage](Tween* t)
		{
			winImage->SetColorA(t->Eased());
		});
		entity->Depth(-999999);
		Add(entity);
		m_gameHasEnded = true;
	}
}

void BulletJamScene::GenerateEnemy(int wave)
{
	STL::Vector<int> typeArray;
	switch (wave)
	{
	case 0:
		typeArray = { 0, 0, 0 };
		break;
	case 1:
		typeArray = { 0, 0, 0, 0, 0, 0, 0 ,0 ,0 ,0 };
		break;
	case 2:
		typeArray = { 1, 1, 1, 1, 1, 1, 1 };
		break;
	case 3:
		typeArray = { 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 };
		break;
	case 4:
		typeArray = { 0, 0, 0, 1, 1, 2, 2, 2, 3 };
		break;
	case 5:
		typeArray = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3 };
		break;
	case 6:
		typeArray = { 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3 };
		break;
	case 7:
		AddBoss();
		break;
	}
	AddEnemy(typeArray);
}

void BulletJamScene::AddBoss()
{
	// TODO:
}

void BulletJamScene::AddEnemy(const STL::Vector<int>& enemyType)
{
	for (auto type : enemyType) {
		AddEnemy(type);
	}
}

void BulletJamScene::AddEnemy(int type)
{
	Math::Vec2 validSpawnPosition = GetValidSpawnPosition();
	AddEnemyAt(type, validSpawnPosition);
}

void BulletJamScene::AddEnemyAt(int type, const Math::Vec2& position)
{
	// Enemy* enemy = Enemy::Create(type);
	// enemy->Position(position);
	// Add(enemy);
	// m_aliveEnemyCounter++;
}

Math::Vec2 BulletJamScene::GetValidSpawnPosition()
{
	return Math::Vec2();
}

void BulletJamScene::OnEnemyDead()
{
	--m_aliveEnemyCounter;
	if (m_aliveEnemyCounter == 0 && !m_enemyTimer->Active()) {
		m_enemyTimer->Start();
	}
}
