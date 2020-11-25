#include "Enemy.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using ld42_bulletjam::Enemy;

Enemy* Enemy::Create(int type)
{
	auto enemy = new Enemy;
	enemy->m_slimeSprite = new Sprite(GameApp::GetAtlas());
	enemy->Add(enemy->m_slimeSprite);
	switch (type) {
	case 0:
		enemy->m_slimeSprite->AddLoop("walk", "slime_walk/slime_walk", 1, 1);
		break;
	case 1:
		enemy->m_slimeSprite->AddLoop("walk", "slimeB_walk/slimeB_walk", 1, 1);
		break;
	case 2:
		enemy->m_slimeSprite->AddLoop("walk", "slimeC_walk/slimeC_walk", 1, 1);
		break;
	case 3:
		enemy->m_slimeSprite->AddLoop("walk", "slimeD_walk/slimeD_walk", 1, 1);
		break;
	case 4:
		enemy->m_slimeSprite->AddLoop("walk", "slimeCC_walk/slimeCC_walk", 1, 1);
		break;
	}
	enemy->m_slimeSprite->Play("walk");
	return enemy;
}
