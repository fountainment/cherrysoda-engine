#include "Bomb.h"

#include "Program.h"
#include "Enemy.h"
#include "Boss.h"

#include <CherrySoda/CherrySoda.h>

using namespace ld42_bulletjam;
using namespace cherrysoda;

Bomb* Bomb::CreateAt(const Math::Vec2& pos2D)
{
	Bomb* bomb = new Bomb();
	bomb->Position2D(pos2D);
	bomb->m_bombTex = GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("bomb_clear/bomb_clear1");
	bomb->m_bombRedTex = GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("bomb_clear/bomb_clear2");
	bomb->m_bombImage = new Image(bomb->m_bombTex);
	bomb->m_bombImage->CenterOrigin();
	bomb->m_boomSprite = new Sprite(GameApp::GetAtlas());
	bomb->m_boomSprite->Add("boom", "bomb_clear_bang/bomb_clear_bang", 1, 1);
	bomb->m_boomSprite->OnFinish([bomb](StringID) { bomb->RemoveSelf(); });
	bomb->m_boomSprite->CenterOrigin();
	bomb->Add(bomb->m_bombImage);
	bomb->Add(bomb->m_boomSprite);
	bomb->Add(Alarm::Create(AlarmMode::Oneshot, [bomb]()
		{
			bomb->Explode();
		}, bomb->m_timeToExplode, true));
	bomb->Add(Alarm::Create(AlarmMode::Oneshot, [bomb]()
		{
			bomb->m_blinkIndex = 0;
			bomb->m_blinkPeriod = 5;
		}, bomb->m_timeToExplode * 0.7f, true));
	bomb->SetCollider(new Circle(bomb->m_explodeRadius));
	return bomb;
}

void Bomb::Update()
{
	base::Update();
	m_blinkIndex++;
	if (m_blinkIndex % (m_blinkPeriod * 2) == 0)
	{
		m_bombImage->Texture(m_bombTex);
		m_blinkIndex = 0;
	}
	else if (m_blinkIndex % m_blinkPeriod == 0)
	{
		m_bombImage->Texture(m_bombRedTex);
	}
	Depth((int)(PositionY() - 700.f));
}

void Bomb::Explode()
{
	m_bombImage->Visible(false);
	m_boomSprite->Play("boom");
	for (auto item : CollideAll(BitTag::Get("deadbullet"))) {
		item->RemoveSelf();
	}
	for (auto item : CollideAll(BitTag::Get("enemy"))) {
		auto enemy = static_cast<Enemy*>(item);
		enemy->Hit(1000, Math_Normalize(item->Position2D() - Position2D()) * 600.f);
	}
	if (Boss::Exists() && CollideCheck(Boss::Instance())) {
		Boss::Instance()->Hit(4000.f);
	}
}
