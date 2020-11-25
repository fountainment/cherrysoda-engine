#include "Bullet.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"
#include "BulletJamScene.h"

using namespace cherrysoda;
using namespace ld42_bulletjam;

static BitTag s_bulletTag("bullet");
static BitTag s_littleBulletTag("littlebullet");
static BitTag s_deadBulletTag("deadbullet");

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

	auto pos2D = Position2D();
	bool outsideFlag = BulletJamScene::CheckOutsideOfPlayZone(pos2D);

	if (TagCheck(s_bulletTag) && outsideFlag) {
		RemoveSelf();
		m_bulletImage->Visible(false);

		STL::Vector<int> array = { -2, -1, 0, 1, 2 };
		for (int arrayi : array)
		{
			Math::Vec2 vector = Calc::SafeNormalize(Calc::Perpendicular(Speed()));
			Bullet* bullet = Create(Position2D() + Speed() / 60.f * 0.5f + (float)arrayi * 10.f * vector, Speed() + (float)arrayi * 120.f * vector);
			// bullet->m_bulletImage->SetColor(m_bulletImage->GetColor());
			// bullet->m_bulletImage->SetTexture(m_bulletImage->GetTexture());
			float num2 = 0.7f;
			switch (Math_Abs(arrayi))
			{
			case 1:
				num2 *= 0.8f;
				break;
			case 2:
				num2 *= 0.6f;
				break;
			}
			float num3 = num2 * 0.5f - 0.05f;
			bullet->m_bulletImage->Scale(Math::Vec2(num2));
			// bullet.Collider = new Circle(BulletImage.Width * num3);
			bullet->RemoveTag(s_bulletTag);
			bullet->AddTag(s_littleBulletTag);
			GetScene()->Add(bullet);
		}
	}
	else if (TagCheck(s_littleBulletTag) && outsideFlag) {
		Speed(Vec2_Zero);
		RemoveTag(s_littleBulletTag);
		AddTag(s_deadBulletTag);
		Active(false);
	}

	Position(Position() + Math::Vec3(Speed(), 0.f) * Engine::Instance()->DeltaTime());
}
