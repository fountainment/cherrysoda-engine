#include <CherrySoda/Components/Graphics/ParticleEmitter.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>

using cherrysoda::ParticleEmitter;

using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::ParticleSystem;
using cherrysoda::ParticleType;


ParticleEmitter::ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Math::Vec2& position, const Math::Vec2& range, int amount, float interval)
	: base(true, false)
{
	m_system = system;
	m_type = type;
	m_position = position;
	m_range = range;
	m_amount = amount;
	m_interval = interval;
}

ParticleEmitter::ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Math::Vec2& position, const Math::Vec2& range, float direction, int amount, float interval)
	: ParticleEmitter(system, type, position, range, amount, interval)
{
	m_direction = direction;
	m_hasDirection = true;
}

ParticleEmitter::ParticleEmitter(ParticleSystem* system, const ParticleType* type, const Entity* track, const Math::Vec2& position, const Math::Vec2& range, float direction, int amount, float interval)
	: ParticleEmitter(system, type, position, range, amount, interval)
{
	m_direction = direction;
	m_hasDirection = true;
	m_track = track;
}

void ParticleEmitter::Emit()
{
	if (m_hasDirection) {
		m_system->Emit(m_type, m_amount, GetEntity()->Position2D() + m_position, m_range, m_direction);
	}
	else {
		m_system->Emit(m_type, m_amount, GetEntity()->Position2D() + m_position, m_range);
	}
}

void ParticleEmitter::Update()
{
	m_timer -= Engine::Instance()->DeltaTime();
	if (m_timer <= 0.f) {
		m_timer = m_interval;
		Emit();
	}
}
