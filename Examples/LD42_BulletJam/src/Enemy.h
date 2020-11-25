#ifndef _BULLETJAM_ENEMY_H_
#define _BULLETJAM_ENEMY_H_

#include <CherrySoda/Entity.h>

namespace cherrysoda {
class Sprite;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Enemy : public cherrysoda::Entity
{
public:
	static Enemy* Create(int type);

private:
	Enemy() {}

	cherrysoda::Sprite* m_slimeSprite = nullptr;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_ENEMY_H_
