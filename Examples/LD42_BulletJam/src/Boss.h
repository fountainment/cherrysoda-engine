#ifndef _BULLETJAM_BOSS_H_
#define _BULLETJAM_BOSS_H_

#include <CherrySoda/Entity.h>

namespace cherrysoda {
class Sprite;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Boss : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;
	void Hit(float damage);
	void Die();

	static Boss* Instance();
	static bool Exists() { return ms_instance != nullptr; }

private:
	Boss() = default;

	static Boss* Create();

	cherrysoda::Sprite* m_bossSprite = nullptr;
	float m_hp = 24000.f;
	bool m_isDead = false;

	static Boss* ms_instance;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_BOSS_H_
