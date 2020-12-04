#include "Bullet.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"
#include "BulletJamScene.h"

using namespace cherrysoda;
using namespace ld42_bulletjam;

static BitTag s_bulletTag("bullet");
static BitTag s_littleBulletTag("littlebullet");
static BitTag s_deadBulletTag("deadbullet");
static BitTag s_enemyBulletTag("enemybullet");

float Bullet::ms_initialScale = 0.5f;

Bullet* Bullet::Create(Math::Vec2 position, Math::Vec2 speed, bool needRandom/* = true*/, bool whiteBullet/* = false*/, bool enemyBullet/* = false*/)
{
	Bullet* bullet = new Bullet();
	if (whiteBullet) {
		bullet->m_bulletImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("bullet_white"));
	}
	else {
		bullet->m_bulletImage = new Image(GameApp::GetAtlas()->GetAtlasSubtextureFromAtlasAt("bullet"));
	}
	bullet->m_bulletImage->Scale(Math::Vec2(ms_initialScale));
	bullet->m_bulletImage->CenterOrigin();
	if (needRandom)
	{
		bullet->m_bulletImage->RotateOnZ(Calc::GetRandom()->NextFloat());
	}
	bullet->Add(bullet->m_bulletImage);
	bullet->SetCollider(new Circle((float)bullet->m_bulletImage->Height() / 2.f));
	bullet->Position(position);
	bullet->Speed(speed);
	if (needRandom)
	{
		bullet->SpeedX(bullet->SpeedX() + Calc::GetRandom()->NextFloat(-0.5f, 0.5f) * 60.f);
		bullet->SpeedY(bullet->SpeedY() + Calc::GetRandom()->NextFloat(-0.5f, 0.5f) * 60.f);
	}
	bullet->Tag(enemyBullet ? s_enemyBulletTag : s_bulletTag);
	return bullet;
}

void Bullet::Destroy(Bullet* bullet)
{
	delete bullet->GetCollider();
	delete bullet->m_bulletImage;
	delete bullet;
}

void Bullet::Update()
{
	base::Update();

	auto pos2D = Position2D();
	Position2D(pos2D + Speed() * Engine::Instance()->DeltaTime());
	bool outsideFlag = BulletJamScene::CheckOutsideOfPlayZone(pos2D);

	if (TagCheck(s_bulletTag) || TagCheck(s_enemyBulletTag)) {
		int collideCount = 0;
		if (!outsideFlag) {
			collideCount = CollideCount(s_deadBulletTag);
		}
		if (outsideFlag || collideCount >= 5) {
			RemoveSelf();
			m_bulletImage->Visible(false);

			STL::Vector<int> array = { -2, -1, 0, 1, 2 };
			for (int arrayi : array)
			{
				Math::Vec2 vector = Calc::SafeNormalize(Calc::Perpendicular(Speed()));
				Bullet* bullet = Create(Position2D() + Speed() / 60.f * 0.5f + (float)arrayi * 10.f * vector, Speed() + (float)arrayi * 120.f * vector);
				bullet->m_bulletImage->SetColor(m_bulletImage->GetColor());
				bullet->m_bulletImage->Texture(m_bulletImage->Texture());
				float scale = 0.7f;
				switch (Math_Abs(arrayi))
				{
				case 1:
					scale *= 0.8f;
					break;
				case 2:
					scale *= 0.6f;
					break;
				}
				float colliderSize = scale * 0.5f - 0.05f;
				bullet->m_bulletImage->Scale(Math::Vec2(scale));
				bullet->GetColliderAs<Circle>()->Radius(bullet->m_bulletImage->Width() * colliderSize);
				bullet->Tag(s_littleBulletTag);
				GetScene()->Add(bullet);
			}
		}
	}
	else if (TagCheck(s_littleBulletTag) && (outsideFlag || CollideCheck(s_deadBulletTag))) {
		Speed(Vec2_Zero);
		Tag(s_deadBulletTag);
		Active(false);
		if (outsideFlag) {
			auto pos2D = Position2D();
			BulletJamScene::CheckOutsideOfPlayZone(pos2D, true);
			Position2D(pos2D);
		}
	}

}

void Bullet::Removed(Scene* scene)
{
	scene->AddActionOnEndOfFrame([this]() { Bullet::Destroy(this); });
}
