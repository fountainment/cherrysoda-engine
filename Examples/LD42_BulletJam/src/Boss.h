#ifndef _BULLETJAM_BOSS_H_
#define _BULLETJAM_BOSS_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {
class Alarm;
class Sprite;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Bullet;

class Boss : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;
	void Hit(float damage);
	void Die();

	void Shoot();

	void StartSuckBullet();
	void StartShoot() { m_canShoot = true; }

	static Boss* Instance();
	static inline bool Exists() { return ms_instance != nullptr; }

private:
	Boss() = default;

	static Boss* Create();

	cherrysoda::Sprite* m_bossSprite = nullptr;
	float m_hp = 24000.f;
	int m_shootCounter = 0;
	bool m_isDead = false;
	bool m_canShoot = false;
	cherrysoda::Alarm* m_startShootBulletTimer = nullptr;
	cherrysoda::Alarm* m_shootBulletTimer = nullptr;
	cherrysoda::STL::List<Bullet*> m_bulletDepo;

	static Boss* ms_instance;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_BOSS_H_
