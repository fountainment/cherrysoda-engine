#ifndef _CHERYSODA_PARTICLE_PARTICLETYPE_H_
#define _CHERYSODA_PARTICLE_PARTICLETYPE_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Graphics/MTexture.h>
#include <CherrySoda/Particles/Particle.h>
#include <CherrySoda/Util/Chooser.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class ParticleType
{
public:
	enum class ColorModes : type::UInt8 { Static, Choose, Blink, Fade };
	enum class FadeModes : type::UInt8 { None, Linear, Late, InAndOut };
	enum class RotationModes : type::UInt8 { None, Random, SameAsDirection };

	ParticleType() = default;

	inline Particle* Create(Particle* particle, const Math::Vec2& position) const
	{
		return Create(particle, position, m_direction);
	}

	inline Particle* Create(Particle* particle, const Math::Vec2& position, const Color& color) const
	{
		return Create(particle, nullptr, position, m_direction, color);
	}

	inline Particle* Create(Particle* particle, const Math::Vec2& position, float direction) const
	{
		return Create(particle, nullptr, position, direction, m_color);
	}

	inline Particle* Create(Particle* particle, const Math::Vec2& position, const Color& color, float direction) const
	{
		return Create(particle, nullptr, position, direction, color);
	}

	Particle* Create(Particle* particle, Entity* entity, const Math::Vec2& position, float direction, const Color& color) const;

	MTexture m_source;
	Chooser<MTexture> m_sourceChooser;
	Color m_color = Color::White;
	Color m_color2 = Color::White;
	ColorModes m_colorMode = ColorModes::Static;
	FadeModes m_fadeMode = FadeModes::None;

	float m_speedMin = 0.f;
	float m_speedMax = 0.f;
	float m_speedMultiplier = 1.f;
	Math::Vec2 m_acceleration = Vec2_Zero;
	float m_friction = 0.f;
	float m_direction = 0.f;
	float m_directionRange = 0.f;
	float m_lifeMin = 0.f;
	float m_lifeMax = 0.f;
	float m_size = 2.f;
	float m_sizeRange = 0.f;
	float m_spinMin = 0.f;
	float m_spinMax = 0.f;
	bool m_spinFlippedChance = false;
	RotationModes m_rotationMode = RotationModes::None;
	bool m_scaleOut = false;
	bool m_useActualDeltaTime = false;
};

} // namespace cherrysoda

#endif // _CHERYSODA_PARTICLE_PARTICLETYPE_H_
