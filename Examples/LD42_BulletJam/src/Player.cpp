#include "Player.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using namespace ld42_bulletjam;

Player* Player::Instance()
{
	static Player* s_instance = nullptr;
	if (s_instance == nullptr) {
		s_instance = Create();
	}
	return s_instance;
}

Player* Player::Create()
{
	Player* player = new Player();
	player->m_bulletAlarm = Alarm::Create(AlarmMode::Persist, [player]()
	{
		player->m_canShoot = true;
	}, 0.1f);
	player->Add(player->m_bulletAlarm);
	player->m_playerFootImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("playerfoot/playerfoot1"));
	player->m_playerFootImage->CenterOrigin();
	player->m_playerImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("player/player1"));
	player->m_playerImage->CenterOrigin();
	player->Add(player->m_playerFootImage);
	player->Add(player->m_playerImage);
	// player->m_collider = new Circle(8f);
	player->Position(Math::Vec2(300.f, 200.f));
	// player->m_hitShaker = new Shaker(false, player.OnShake);
	// player->Add(player->m_hitShaker);
	player->m_HP = 6;
	return player;
}

void Player::Update()
{
	base::Update();

	Math::Vec2 move(0.f);
	if (MInput::Keyboard()->Check(Keys::W))
	{
		move.y += 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::A))
	{
		move.x -= 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::S))
	{
		move.y -= 1.f;
	}
	if (MInput::Keyboard()->Check(Keys::D))
	{
		move.x += 1.f;
	}
	if (move != Vec2_Zero) {
		move = Math_Normalize(move);
	}
	Position(Position() + Engine::Instance()->DeltaTime() * 180.f * Math::Vec3(move, 0.f));
	Depth(Position().x - 700.0f);
}
