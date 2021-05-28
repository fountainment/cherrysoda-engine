#ifndef _CHERYSODA_PARTICLE_PARTICLE_H_
#define _CHERYSODA_PARTICLE_PARTICLE_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Entity;
class MTexture;
class ParticleType;

struct Particle
{
	const Entity* m_track;
	const ParticleType* m_type;
	const MTexture* m_source;

	bool m_active;
	Color m_color;
	Color m_startColor;
	Math::Vec2 m_position;
	Math::Vec2 m_speed;
	float m_size;
	float m_startSize;
	float m_life;
	float m_startLife;
	float m_colorSwitch;
	float m_rotation;
	float m_spin;

	void Update();
	void Update(float dt);
	void Render();
};

} // namespace cherrysoda

#endif // _CHERYSODA_PARTICLE_PARTICLE_H_
