#include <CherrySoda/Particles/ParticleType.h>

#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::ParticleType;

using cherrysoda::Calc;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::Particle;


Particle* ParticleType::Create(Particle* particle, Entity* entity, const Math::Vec2& position, float direction, const Color& color) const
{
	particle->m_track = entity;
	particle->m_type = this;
	particle->m_active = true;
	particle->m_position = position;

	// source texture
	if (!m_sourceChooser.IsEmpty()) {
		particle->m_source = &m_sourceChooser.Choose();
	}
	else if (m_source.IsValid()) {
		particle->m_source = &m_source;
	}
	else {
		particle->m_source = &Draw::ParticleTexture();
	}

	// size
	if (m_sizeRange != 0.f) {
		particle->m_startSize = particle->m_size = m_size - m_sizeRange * .5f + Calc::GetRandom()->NextFloat(m_sizeRange);
	}
	else {
		particle->m_startSize = particle->m_size = m_size;
	}

	// color
	if (m_colorMode == ParticleType::ColorModes::Choose) {
		particle->m_startColor = particle->m_color = Calc::GetRandom()->Choose(color, m_color2);
	}
	else {
		particle->m_startColor = particle->m_color = color;
	}

	// speed / direction
	float moveDirection = direction - m_directionRange * .5f + Calc::GetRandom()->NextFloat(m_directionRange);
	particle->m_speed = Calc::AngleToVector(moveDirection, Calc::GetRandom()->NextFloat(m_speedMin, m_speedMax));

	// life
	particle->m_startLife = particle->m_life = Calc::GetRandom()->NextFloat(m_lifeMin, m_lifeMax);

	// rotation
	if (m_rotationMode == ParticleType::RotationModes::Random) {
		particle->m_rotation = Calc::GetRandom()->NextAngle();
	}
	else if (m_rotationMode == ParticleType::RotationModes::SameAsDirection) {
		particle->m_rotation = moveDirection;
	}
	else {
		particle->m_rotation = 0.f;
	}

	// spin
	particle->m_spin = Calc::GetRandom()->NextFloat(m_spinMin, m_spinMax);
	if (m_spinFlippedChance) {
		particle->m_spin *= Calc::GetRandom()->Choose(1, -1);
	}

	return particle;
}
