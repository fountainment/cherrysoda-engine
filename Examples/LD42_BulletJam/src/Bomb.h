#ifndef _BULLETJAM_BOMB_H_
#define _BULLETJAM_BOMB_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {
class Image;
} // namespace cherrysoda

namespace ld42_bulletjam {

class Bomb : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;

	void Update() override;

	static Bomb* CreateAt(const cherrysoda::Math::Vec2& pos2D);

private:
	Bomb() = default;

	void Explode();

	int m_blinkIndex = 0;
	int m_blinkPeriod = 8;
	cherrysoda::Image* m_bombImage = nullptr;
	cherrysoda::MTexture m_bombTex;
	cherrysoda::MTexture m_bombRedTex;

	const float m_timeToExplode = 3.f;
	const float m_explodeRadius = 100.f;
};

} // namespace ld42_bulletjam

#endif // _BULLETJAM_BOMB_H_
