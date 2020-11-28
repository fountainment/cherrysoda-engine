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
	enemy->m_slimeType = type;
	float colliderSize = 8.f;
	switch (type) {
	case 0:
		enemy->m_hp = 100;
		enemy->m_jamSize = 1.f;
		enemy->m_jamColor = Color(55, 148, 110);
		enemy->m_slimeSprite->AddLoop("walk", "slime_walk/slime_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize));
		break;
	case 1:
		enemy->m_hp = 200;
		enemy->m_jamSize = 1.5f;
		enemy->m_jamColor = Color(227, 186, 50);
		enemy->m_slimeSprite->AddLoop("walk", "slimeB_walk/slimeB_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.f));
		break;
	case 2:
		enemy->m_hp = 150;
		enemy->m_jamSize = 2.f;
		enemy->m_jamColor = Color(179, 70, 139);
		enemy->m_slimeSprite->AddLoop("walk", "slimeC_walk/slimeC_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.5f));
		break;
	case 3:
		enemy->m_hp = 150;
		enemy->m_jamSize = 2.f;
		enemy->m_jamColor = Color(227, 186, 50);
		enemy->m_slimeSprite->AddLoop("walk", "slimeD_walk/slimeD_walk", 1, 1);
		enemy->SetCollider(new Circle(colliderSize * 2.5f));
		break;
	case 4:
		enemy->m_hp = 100;
		enemy->m_jamSize = 1.f;
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
			STL::Vector<int> array = { -3, -2, -1, 0, 1, 2, 3 };
			for (int arrayi : array)
			{
				Math::Vec2 vector = Calc::SafeNormalize(Calc::Perpendicular(m_deadSpeed));
				Bullet* bullet = Bullet::Create(Position2D() + m_deadSpeed / 60.f * 0.5f + (float)arrayi * 10.f * m_jamSize * vector,  m_deadSpeed + (float)arrayi * 120.f * vector, true, true);
				bullet->BulletImage()->SetColor(m_jamColor);
				float scale = m_jamSize;
				switch (Math_Abs(arrayi))
				{
				case 1:
					scale *= 0.8f;
					break;
				case 2:
					scale *= 0.7f;
					break;
				case 3:
					scale *= 0.3f;
					break;
				}
				float colliderSize = scale * 0.5f - 0.05f;
				bullet->BulletImage()->Scale(Math::Vec2(scale));
				bullet->GetColliderAs<Circle>()->Radius(bullet->BulletImage()->Width() * colliderSize);
				bullet->Tag(BitTag::Get("littlebullet"));
				GetScene()->Add(bullet);
			}
			RemoveSelf();
		}
	}
	else {
		Math::Vec2 speed = Calc::SafeNormalize(Player::Instance()->Position() - Position(), Vec2_YUp) * 42.f;

		auto bulletList = CollideAll(BitTag::Get("bullet"));
		if (STL::IsNotEmpty(bulletList)) {
			for (auto entity : bulletList) {
				auto bullet = (Bullet*)entity;
				Hit(bullet->Damage(), bullet->Speed());
				bullet->RemoveSelf();
			}
		}
		else {
			Math::Vec2 move = speed * Engine::Instance()->DeltaTime();
			Position2D(Position2D() + move);

			STL::List<Entity*> actorList = CollideAll(s_enemyTag);
			if (Player::Exists() && CollideCheck(Player::Instance()))
			{
				Player::Instance()->Hit();
				STL::Add(actorList, Player::Instance());
			}
			if (STL::IsNotEmpty(actorList))
			{
				Position2D(Position2D() - move);
			}
			actorList = CollideAll(s_enemyTag);
			if (Player::Exists() && CollideCheck(Player::Instance()))
			{
				STL::Add(actorList, Player::Instance());
			}
			Entity* entity = nullptr;
			if (STL::TryGetFirst(actorList, entity))
			{
				Math::Vec2 vec = Position2D() - entity->Position2D();
				vec = Calc::SafeNormalize(vec, Vec2_XUp);
				Position2D(Position2D() + vec * 10.f);
			}
		}
	}

	Depth(Math_Round(PositionY() - 700.f));
}

void Enemy::Hit(int damage, Math::Vec2 speed)
{
	m_hp -= damage;
	m_deadSpeed = speed;
	if (m_hp < 0) {
		Die();
	}
}

void Enemy::Die()
{
	if (m_isDead) {
		return;
	}
	m_isDead = true;
	switch (m_slimeType) {
	case 2:
		{
			Math::Vec2 vector = Calc::SafeNormalize(Calc::Perpendicular(m_deadSpeed), Vec2_XUp);
			GetSceneAs<BulletJamScene>()->AddEnemyAt(4, Position2D() + vector * 20.f);
			GetSceneAs<BulletJamScene>()->AddEnemyAt(4, Position2D() + vector * -20.f);
			RemoveSelf();
		}
	case 3:
		{
			RemoveSelf();
		}
	default:
		m_slimeSprite->Stop();
		break;
	}
	GetSceneAs<BulletJamScene>()->OnEnemyDead();
}

void Enemy::Removed(Scene* scene)
{
	scene->AddActionOnEndOfFrame([this]() { Enemy::Destroy(this); });
}
