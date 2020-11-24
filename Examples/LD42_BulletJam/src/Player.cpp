#include "Player.h"

#include "Program.h"
#include "Bullet.h"
#include "Cursor.h"

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
	player->m_playerFootSprite = new Sprite(GameApp::GetAtlas());
	player->m_playerFootSprite->AddLoop("stand", "playerfoot/playerfoot1");
	player->m_playerFootSprite->AddLoop("walk", "playerfoot/playerfoot", 1, 1);
	player->m_playerFootSprite->Play("stand");
	player->m_playerFootSprite->CenterOrigin();
	player->m_playerFootSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	player->m_playerSprite = new Sprite(GameApp::GetAtlas());
	player->m_playerSprite->AddLoop("right",     "player/player1");
	player->m_playerSprite->AddLoop("rightdown", "player/player2");
	player->m_playerSprite->AddLoop("down",      "player/player3");
	player->m_playerSprite->AddLoop("leftdown",  "player/player4");
	player->m_playerSprite->AddLoop("left",      "player/player5");
	player->m_playerSprite->AddLoop("leftup",    "player/player6");
	player->m_playerSprite->AddLoop("up",        "player/player7");
	player->m_playerSprite->AddLoop("rightup",   "player/player8");
	player->m_playerSprite->Play("right");
	player->m_playerSprite->CenterOrigin();
	player->m_playerSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	player->Add(player->m_playerFootSprite);
	player->Add(player->m_playerSprite);
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

	Math::Vec2 vector = Calc::EightWayNormal(Math::Vec2(Math_Normalize(Cursor::Instance()->Position() - Position())));
	if (vector == Vec2_YUp)
	{
		m_playerSprite->Play("up");
	}
	else if (vector == -Vec2_YUp)
	{
		m_playerSprite->Play("down");
	}
	else if (vector == -Vec2_XUp)
	{
		m_playerSprite->Play("left");
	}
	else if (vector == Vec2_XUp)
	{
		m_playerSprite->Play("right");
	}
	else if (vector.x == -vector.y && vector.x < 0.f)
	{
		m_playerSprite->Play("leftup");
	}
	else if (vector.x == -vector.y && vector.x > 0.f)
	{
		m_playerSprite->Play("rightdown");
	}
	else if (vector.x == vector.y && vector.x < 0.f)
	{
		m_playerSprite->Play("leftdown");
	}
	else if (vector.x == vector.y && vector.x > 0.f)
	{
		m_playerSprite->Play("rightup");
	}

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
	move = Calc::SafeNormalize(move);
	if (move == Vec2_Zero) {
		m_playerFootSprite->Play("stand");
	}
	else 
	{
		m_playerFootSprite->Play("walk");
		if (move.x > 0.f) {
			m_playerFootSprite->FlipX(false);
		}
		else if (move.x < 0.f) {
			m_playerFootSprite->FlipX(true);
		}
	}
	Move(move);
	Depth(Position().x - 700.0f);

	if (MInput::Mouse()->CheckLeftButton())
	{
		Shoot();
	}
}

void Player::Move(const Math::Vec2& move)
{
	Position(Position() + Engine::Instance()->DeltaTime() * 180.f * Math::Vec3(move, 0.f));
	if (PositionY() < 80.f)
	{
		PositionY(80.f);
	}
	if (PositionX() < 80.f)
	{
		PositionX(80.f);
	}
	if (PositionY() > 520.f)
	{
		PositionY(520.f);
	}
	if (PositionX() > 520.f)
	{
		PositionX(520.f);
	}
}

void Player::Shoot()
{
	if (m_canShoot)
	{
		GetScene()->Add(Bullet::Create(Position(), 600.f * Math_Normalize(Cursor::Instance()->Position() - Position())));
		m_canShoot = false;
		m_bulletAlarm->Start();
	}
}
