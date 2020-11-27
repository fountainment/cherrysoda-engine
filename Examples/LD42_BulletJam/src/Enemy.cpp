#include "Enemy.h"

#include "Program.h"
#include "Bullet.h"
#include "Player.h"
#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using namespace ld42_bulletjam;

BitTag s_enemyTag("enemy");

Enemy* Enemy::Create(int type)
{
	auto enemy = new Enemy;
	enemy->m_slimeSprite = new Sprite(GameApp::GetAtlas());
	enemy->m_slimeSprite->CenterOrigin();
	enemy->Add(enemy->m_slimeSprite);
	float colliderSize = 8.f;
	switch (type) {
	case 0:
		enemy->m_hp = 100;
		enemy->m_jamColor = Color(55, 148, 110);
		enemy->m_slimeSprite->AddLoop("walk", "slime_walk/slime_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize));
		break;
	case 1:
		enemy->m_hp = 200;
		enemy->m_jamColor = Color(227, 186, 50);
		enemy->m_slimeSprite->AddLoop("walk", "slimeB_walk/slimeB_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.f));
		break;
	case 2:
		enemy->m_hp = 150;
		enemy->m_jamColor = Color(179, 70, 139);
		enemy->m_slimeSprite->AddLoop("walk", "slimeC_walk/slimeC_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.5f));
		break;
	case 3:
		enemy->m_hp = 150;
		enemy->m_jamColor = Color(227, 186, 50);
		enemy->m_slimeSprite->AddLoop("walk", "slimeD_walk/slimeD_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.5f));
		break;
	case 4:
		enemy->m_hp = 100;
		enemy->m_jamColor = Color(179, 70, 139);
		enemy->m_slimeSprite->AddLoop("walk", "slimeCC_walk/slimeCC_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize));
		break;
	}
	enemy->m_slimeSprite->Play("walk", false, true);
	enemy->m_slimeSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	enemy->Tag(s_enemyTag);
	return enemy;
}

void Enemy::Destroy(Enemy* enemy)
{
	delete enemy->m_slimeSprite;	
	delete enemy->GetCollider();
	delete enemy;
}

void Enemy::Update()
{
	base::Update();

	if (m_isDead) {
		Position2D(Position2D() + m_deadSpeed * Engine::Instance()->DeltaTime());

		auto pos2D = Position2D();
		if (BulletJamScene::CheckOutsideOfPlayZone(pos2D, true) || CollideCheck(BitTag::Get("deadbullet")))
		{
			Position2D(pos2D);
			RemoveSelf();
		}
	}
	else {
		Math::Vec2 vec2 = Calc::SafeNormalize(Player::Instance()->Position() - Position(), Vec2_YUp) * 42.f;
		Position2D(Position2D() + vec2 * Engine::Instance()->DeltaTime());

		auto list = CollideAll(BitTag::Get("bullet"));
		for (auto entity : list) {
			auto bullet = (Bullet*)entity;
			Hit(bullet->Damage(), bullet->Speed());
			bullet->RemoveSelf();
		}
	}

	Depth(Math_Round(PositionY() - 700.f));
}

void Enemy::Hit(int damage, Math::Vec2 speed)
{
	m_hp -= damage;
	m_deadSpeed = speed;
	if (m_hp < 0) {
		Dead();
	}
}

void Enemy::Dead()
{
	if (m_isDead) {
		return;
	}
	m_isDead = true;
	m_slimeSprite->Stop();
	GetSceneAs<BulletJamScene>()->OnEnemyDead();
}

void Enemy::Removed(Scene* scene)
{
	scene->AddActionOnEndOfFrame([this]() { Enemy::Destroy(this); });
}
