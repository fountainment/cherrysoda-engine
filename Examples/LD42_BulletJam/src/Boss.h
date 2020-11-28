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
	static Boss* Instance();

private:
	Boss() {}

	static Boss* Create();

	cherrysoda::Sprite* m_bossSprite = nullptr;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_BOSS_H_
