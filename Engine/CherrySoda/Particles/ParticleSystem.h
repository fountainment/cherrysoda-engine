#ifndef _CHERYSODA_PARTICLE_PARTICLESYSTEM_H_
#define _CHERYSODA_PARTICLE_PARTICLESYSTEM_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Particles/Particle.h>
#include <CherrySoda/Particles/ParticleType.h>
#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class ParticleSystem : public Entity
{
public:
	ParticleSystem(int depth, int maxParticles)
	{
		STL::Resize(m_particles, maxParticles);
		Depth(depth);
	}

	void Clear()
	{
		for (auto& particle : m_particles) {
			particle.m_active = false;
		}
	}

	void Update() override
	{
		CHERRYSODA_PROFILE_FUNCTION();

		for (auto& particle : m_particles) {
			if (particle.m_active) {
				particle.Update();
			}
		}
	}

	void Render() override
	{
		CHERRYSODA_PROFILE_FUNCTION();

		for (auto& particle : m_particles) {
			if (particle.m_active) {
				particle.Render();
			}
		}
	}

	// For particle editor
	int ActiveAmount()
	{
		int amount = 0;
		for (auto& particle : m_particles) {
			if (particle.m_active) amount++;
		}
		return amount;
	}

	void Emit(const ParticleType* type, const Math::Vec2& position)
	{
		type->Create(&m_particles[m_nextSlot], position);
		m_nextSlot = (m_nextSlot + 1) % STL::Count(m_particles);
	}

	void Emit(const ParticleType* type, const Math::Vec2& position, float direction)
	{
		type->Create(&m_particles[m_nextSlot], position, direction);
		m_nextSlot = (m_nextSlot + 1) % STL::Count(m_particles);
	}

	void Emit(const ParticleType* type, const Math::Vec2& position, const Color& color)
	{
		type->Create(&m_particles[m_nextSlot], position, color);
		m_nextSlot = (m_nextSlot + 1) % STL::Count(m_particles);
	}

	void Emit(const ParticleType* type, const Math::Vec2& position, const Color& color, float direction)
	{
		type->Create(&m_particles[m_nextSlot], position, color, direction);
		m_nextSlot = (m_nextSlot + 1) % STL::Count(m_particles);
	}

	void Emit(const ParticleType* type, int amount, const Math::Vec2& position, const Math::Vec2& positionRange)
	{
		for (int i = 0; i < amount; ++i) {
			Emit(type, Calc::GetRandom()->Range(position - positionRange, position + positionRange));
		}
	}

	void Emit(const ParticleType* type, int amount, const Math::Vec2& position, const Math::Vec2& positionRange, float direction)
	{
		for (int i = 0; i < amount; ++i) {
			Emit(type, Calc::GetRandom()->Range(position - positionRange, position + positionRange), direction);
		}
	}

private:
	STL::Vector<Particle> m_particles;
	int m_nextSlot = 0;
};

} // namespace cherrysoda

#endif // _CHERYSODA_PARTICLE_PARTICLESYSTEM_H_
