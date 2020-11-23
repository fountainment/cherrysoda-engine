#ifndef _BULLETJAM_PLAYER_H_
#define _BULLETJAM_PLAYER_H_

#include <CherrySoda/Entity.h>

namespace cherrysoda {
class Alarm;
class Image;
};

namespace ld42_bulletjam {

class Player : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;

	static Player* Instance();	
	static Player* Create();


private:
	cherrysoda::Alarm* m_bulletAlarm;
	cherrysoda::Image* m_playerFootImage;
	cherrysoda::Image* m_playerImage;
	bool m_canShoot = false;
	int m_HP;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_PLAYER_H_
