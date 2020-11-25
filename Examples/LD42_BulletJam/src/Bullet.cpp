#include "Bullet.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"

using namespace cherrysoda;

using ld42_bulletjam::Bullet;

static BitTag s_bulletTag("bullet");

float Bullet::ms_initialScale = 0.5f;

Bullet* Bullet::Create(Math::Vec2 position, Math::Vec2 speed, bool needRandom/* = true*/)
{
	Bullet* bullet = new Bullet();
	bullet->m_bulletImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("bullet"));
	bullet->m_bulletImage->Scale(Math::Vec2(ms_initialScale));
	bullet->m_bulletImage->CenterOrigin();
	if (needRandom)
	{
		bullet->m_bulletImage->RotateOnZ(Calc::GetRandom()->NextFloat());
	}
	bullet->Add(bullet->m_bulletImage);
	// bullet->m_collider = new Circle((float)bullet->m_bulletImage->Height() / 2.f);
	bullet->Position(position);
	if (speed == Vec2_Zero) {
		speed = Vec2_YUp * 10.f;
	}
	bullet->Speed(speed);
	if (needRandom)
	{
		bullet->SpeedX(bullet->SpeedX() + Calc::GetRandom()->NextFloat() - 0.5f);
	}
	bullet->AddTag(s_bulletTag);
	return bullet;
}

void Bullet::Update()
{
	base::Update();

	Position(Position() + Math::Vec3(Speed(), 0.f) * Engine::Instance()->DeltaTime());
}
