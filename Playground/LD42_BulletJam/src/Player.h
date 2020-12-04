#ifndef _BULLETJAM_PLAYER_H_
#define _BULLETJAM_PLAYER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {
class Alarm;
class Shaker;
class Sprite;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Player : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;

	void Move(const cherrysoda::Math::Vec2& move);
	void Shoot();
	void PlaceBomb();
	void Hit();
	void Die();

	static Player* Instance();
	static inline bool Exists() { return ms_instance != nullptr; }

private:
	Player() = default;
	static Player* Create();

	cherrysoda::Alarm* m_bulletAlarm = nullptr;
	cherrysoda::Sprite* m_playerFootSprite = nullptr;
	cherrysoda::Sprite* m_playerSprite = nullptr;
	cherrysoda::Shaker* m_hitShaker = nullptr;
	bool m_canShoot = true;
	bool m_POW = false;
	bool m_isDead = false;
	int m_HP = 0;
	int m_bombCount = 3;

	static Player* ms_instance;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_PLAYER_H_
