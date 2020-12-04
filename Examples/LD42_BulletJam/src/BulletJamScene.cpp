#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"
#include "Player.h"
#include "Cursor.h"
#include "Enemy.h"
#include "Boss.h"

using namespace cherrysoda;
using namespace ld42_bulletjam;

// It's the code port from the original BulletJam in LD42 which is written in C# and with XNA.
// And the original code can be find here: https://github.com/fountainment/LD42_BulletJam
// The structure is currently quite messy, I may clean it up in the future.

void BulletJamScene::Begin()
{
	// Renderer
	m_renderer = new EverythingRenderer();
	Add(m_renderer);
	// Renderer effect
	m_renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	// Renderer camera
	auto halfViewSize = Math::Vec2(300.f, 350.f);
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
	if (MInput::Keyboard()->Pressed(Keys::Space) || MInput::Mouse()->PressedLeftButton())
	{
		GameRestart();
		GameStart();
	}

	// TODO: Consider moving these to Camera
	m_renderer->GetCamera()->SetSize(Engine::Instance()->GetViewSize());
	Math::IVec2 winSize = Engine::Instance()->GetWindowSize();
	float ratio = winSize.x / (float)winSize.y;
	if (ratio < 600.f / 700.f) {
		m_renderer->GetCamera()->PositionZ(300.f / ratio);
	}
	else {
		m_renderer->GetCamera()->PositionZ(350.f);
	}

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
		Add(Cursor::Instance());
		m_enemyTimer->Start();	
		m_openUITween->Start();
	}
}

void BulletJamScene::GameOver()
{
	if (!m_gameHasEnded)
	{
		Tween* tween = Tween::Create(TweenMode::Oneshot, Ease::QuadInOut, 0.8f, true);
		Entity* entity = new Entity();
		Image* loseImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("loseUI"));
		loseImage->SetColor(Color::White * 0.f);
		entity->Add(loseImage);
		entity->Add(tween);
		tween->OnUpdate([loseImage](Tween* t){
			// For premultiplied-alpha, the color rgb need to multiply alpha value
			loseImage->SetColor(Color::White * t->Eased());
		});
		entity->Depth(-999999);
		Add(entity);
		m_gameHasEnded = true;
	}
}

void BulletJamScene::Win()
{
	if (!m_gameHasEnded)
	{
		Tween* tween = Tween::Create(TweenMode::Oneshot, Ease::QuadInOut, 0.8f, true);
		Entity* entity = new Entity();
		Image* winImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("winUI"));
		winImage->SetColor(Color::White * 0.f);
		entity->Add(winImage);
		entity->Add(tween);
		tween->OnUpdate([winImage](Tween* t)
		{
			// For premultiplied-alpha, the color rgb need to multiply alpha value
			winImage->SetColor(Color::White * t->Eased());
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
	auto boss = Boss::Instance();
	auto pos2D = boss->Position2D();
	pos2D = GetValidSpawnPosition();
	CheckOutsideOfPlayZone(pos2D, true, 100.f);
	boss->Position2D(pos2D);
	Add(boss);

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
	Enemy* enemy = Enemy::Create(type);
	enemy->Position(position);
	Add(enemy);
	m_aliveEnemyCounter++;
}

bool BulletJamScene::CheckOutsideOfPlayZone(Math::Vec2& position, bool normalize/* = false*/, float margin/* = 0.f*/)
{
	const float rangeMin = 50.f + margin;
	const float rangeMax = 550.f - margin;
	bool result = position.x < rangeMin || position.y < rangeMin || position.x > rangeMax || position.y > rangeMax;
	if (normalize)
	{
		if (position.x < rangeMin)
		{
			position.x = rangeMin;
		}
		if (position.y < rangeMin)
		{
			position.y = rangeMin;
		}
		if (position.x > rangeMax)
		{
			position.x = rangeMax;
		}
		if (position.y > rangeMax)
		{
			position.y = rangeMax;
		}
	}
	return result;
}

Math::Vec2 BulletJamScene::GetValidSpawnPosition()
{
	float num = Calc::GetRandom()->NextAngle();
	Math::Vec2 vector = Math::Vec2((float)Math_Cos(num), (float)Math_Sin(num));
	Math::Vec2 position = LineWalkCheck(Player::Instance()->Position2D(), Player::Instance()->Position2D() + vector * 500.f, BitTag::Get("deadbullet").ID(), 5.f);
	CheckOutsideOfPlayZone(position, true);
	return position + (Player::Instance()->Position2D() - position) * 0.1f;
}

void BulletJamScene::OnEnemyDead()
{
	--m_aliveEnemyCounter;
	if (m_aliveEnemyCounter == 0 && !m_enemyTimer->Active()) {
		m_enemyTimer->Start();
	}
}

void BulletJamScene::SetPlayerHP(int hp)
{
	if (m_playerHPImage) {
		m_playerHPImage->Texture(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt(CHERRYSODA_FORMAT("player_HP/player_HP%d", 7 - hp)));
	}
}

void BulletJamScene::SetPlayerBomb(int bombCount)
{
	if (m_playerBombImage) {
		m_playerBombImage->Texture(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt(CHERRYSODA_FORMAT("bomb_num/bomb_num%d", 4 - bombCount)));
	}
}
