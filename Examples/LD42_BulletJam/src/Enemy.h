#ifndef _BULLETJAM_ENEMY_H_
#define _BULLETJAM_ENEMY_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {
class Sprite;
class Color;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Enemy : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;

	void Hit(int damage, cherrysoda::Math::Vec2 speed);
	void Dead();

	static Enemy* Create(int type);
	static void Destroy(Enemy* Enemy);

private:
	Enemy() {}

	void Removed(cherrysoda::Scene* scene) override;

	cherrysoda::Sprite* m_slimeSprite = nullptr;
	cherrysoda::Math::Vec2 m_deadSpeed = Vec2_Zero;
	cherrysoda::Color m_jamColor = cherrysoda::Color::White;
	int m_hp = 100;
	int m_slimeType = 0;
	float m_jamSize = 1.f;
	bool m_isDead = false;

};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_ENEMY_H_
