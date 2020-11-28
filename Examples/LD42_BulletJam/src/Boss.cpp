#include "Boss.h"

#include "Program.h"
#include "Bullet.h"
#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace ld42_bulletjam;
using namespace cherrysoda;

BitTag s_suckBulletTag("suckbullet");
BitTag s_depoBulletTag("depobullet");

Boss* Boss::Instance()
{
	if (ms_instance == nullptr) {
		ms_instance = Boss::Create();
	}
	return ms_instance;
}

Boss* Boss::Create()
{
	Boss* boss = new Boss();
	boss->m_bossSprite = new Sprite(GameApp::GetAtlas());
	boss->m_bossSprite->CenterOrigin();
	boss->m_bossSprite->AddLoop("walk", "boss_walk/boss_walk", 1, 1);
	boss->m_bossSprite->Add("die", "boss_die/boss_die", 1, 1);
	boss->m_bossSprite->Play("walk");
	boss->m_bossSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	boss->m_startShootBulletTimer = Alarm::Create(AlarmMode::Oneshot, [boss](){ boss->StartShoot(); }, 3.f);
	boss->m_shootBulletTimer = Alarm::Create(AlarmMode::Looping, [boss](){ boss->Shoot(); }, 0.5f, true);
	boss->Add(boss->m_bossSprite);
	boss->Add(boss->m_startShootBulletTimer);
	boss->Add(boss->m_shootBulletTimer);
	boss->Add(Alarm::Create(AlarmMode::Oneshot, [boss](){ boss->StartSuckBullet(); }, 3.f, true));
	boss->SetCollider(new Circle(80.f));
	return boss;
}

void Boss::Update()
{
	base::Update();

	STL::List<Entity*> list = (*GetScene())[s_suckBulletTag];
	for (auto item : list)
	{
		auto vec = Position2D() - item->Position2D();
		float distSq = Math_LengthSq(vec);
		float scale = Calc::GetRandom()->NextFloat() * 6000.f / distSq;
		if (scale > 1.f)
		{
			item->Tag(s_depoBulletTag);
			item->Active(false);
			item->Visible(false);
		}
		else
		{
			const Math::Vec2 speed = scale * vec * 60.f;
			item->Position2D(item->Position2D() + (speed * Engine::Instance()->DeltaTime()));
		}
	}

	for (auto entity : CollideAll(BitTag::Get("bullet")))
	{
		auto bullet = static_cast<Bullet*>(entity);
		Hit(bullet->Damage());
		bullet->RemoveSelf();
		if (m_isDead)
		{
			break;
		}
	}
}

void Boss::Hit(float damage)
{
	m_hp -= damage;

	if (m_hp < 0) {
		Die();
	}
}

void Boss::Die()
{
	if (m_isDead) {
		return;
	}
	m_isDead = true;

	m_bossSprite->Play("die");
	m_bossSprite->OnFinish([this](StringID id) { if (id == StringID("die")) { GetSceneAs<BulletJamScene>()->Win(); } });
}

void Boss::StartSuckBullet()
{
	STL::List<Entity*> list = (*GetScene())[BitTag::Get("deadbullet")];
	for (auto item : list)
	{
		item->Tag(s_suckBulletTag);
	}
	m_startShootBulletTimer->Start();
}

void Boss::Shoot()
{
	if (m_startShoot) {

	}
}

Boss* Boss::ms_instance = nullptr;
