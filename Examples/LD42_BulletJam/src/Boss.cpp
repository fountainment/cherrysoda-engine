#include "Boss.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace ld42_bulletjam;
using namespace cherrysoda;

Boss* Boss::Instance()
{
	static Boss* s_instance = nullptr;
	if (s_instance == nullptr) {
		s_instance = Boss::Create();
	}
	return s_instance;
}

Boss* Boss::Create()
{
	Boss* boss = new Boss();
	boss->m_bossSprite = new Sprite(GameApp::GetAtlas());
	boss->m_bossSprite->CenterOrigin();
	boss->m_bossSprite->AddLoop("walk", "boss_walk/boss_walk", 1, 1);
	boss->m_bossSprite->Play("walk");
	boss->m_bossSprite->SetSpriteEffects(SpriteEffects::RoundRenderingPosition);
	boss->Add(boss->m_bossSprite);
	return boss;
}
