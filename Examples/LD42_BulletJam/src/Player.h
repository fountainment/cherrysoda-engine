#ifndef _BULLETJAM_PLAYER_H_
#define _BULLETJAM_PLAYER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {
class Alarm;
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

	static Player* Instance();	

private:
	Player() {}
	static Player* Create();

	cherrysoda::Alarm* m_bulletAlarm = nullptr;
	cherrysoda::Sprite* m_playerFootSprite = nullptr;
	cherrysoda::Sprite* m_playerSprite = nullptr;
	bool m_canShoot = true;
	int m_HP = 0;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_PLAYER_H_
