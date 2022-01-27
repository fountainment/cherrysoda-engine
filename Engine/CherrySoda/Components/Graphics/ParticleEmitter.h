#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_PARTICLEEMITTER_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_PARTICLEEMITTER_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Particles/ParticleSystem.h>
#include <CherrySoda/Particles/ParticleType.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class ParticleEmitter : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(ParticleEmitter, Component);

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Interval, m_interval);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(int, Amount, m_amount);
	CHERRYSODA_GETTER_SETTER_OF_VEC2(PositionRange, m_range);

	ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Math::Vec2& position, const Math::Vec2& range, int amount, float interval);
	ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Math::Vec2& position, const Math::Vec2& range, float direction, int amount, float interval);
	ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Entity* track, const Math::Vec2& position, const Math::Vec2& range, float direction, int amount, float interval);

	void Emit();
	void Update() override;

private:
	ParticleSystem* m_system;
	const ParticleType* m_type;

	const Entity* m_track;
	float m_interval;
	Math::Vec2 m_position;
	Math::Vec2 m_range;
	int m_amount;
	bool m_hasDirection = false;
	float m_direction;
	float m_timer = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_PARTICLEEMITTER_H_
