#include "Boss.h"

#include "Program.h"
#include "Bullet.h"
#include "BulletJamScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace ld42_bulletjam;
using namespace cherrysoda;

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
	boss->Add(boss->m_bossSprite);
	boss->SetCollider(new Circle(80.f));
	return boss;
}

void Boss::Update()
{
	base::Update();

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

Boss* Boss::ms_instance = nullptr;
