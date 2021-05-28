#include <CherrySoda/Particles/Particle.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/Graphics/SpriteEffects.h>
#include <CherrySoda/Particles/ParticleType.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Ease.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Particle;

using cherrysoda::Calc;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Ease;
using cherrysoda::Engine;
using cherrysoda::Math;
using cherrysoda::ParticleType;
using cherrysoda::SpriteEffects;


void Particle::Update()
{
	Update(m_type->m_useActualDeltaTime ? Engine::Instance()->RawDeltaTime() : Engine::Instance()->DeltaTime());
}

void Particle::Update(float dt)
{
	float ease = m_life / m_startLife;
	
	// life
	m_life -= dt;
	if (m_life < 0.f) {
		m_life = 0.f;
		m_active = false;
		return;
	}

	// spin
	if (m_type->m_rotationMode == ParticleType::RotationModes::SameAsDirection) {
		if (m_speed != Vec2_Zero) {
			m_rotation = Calc::Angle(m_speed);
		}
		else {
			m_rotation += m_spin * dt;
		}
	}

	// fade
	float alpha;
	switch (m_type->m_fadeMode) {
	case ParticleType::FadeModes::Linear:
		alpha = ease;
		break;
	case ParticleType::FadeModes::Late:
		alpha = Math_Min(1.f, ease / .25f);
		break;
	case ParticleType::FadeModes::InAndOut:
		{
			if (ease > .75f) {
				alpha = 1.f - ((ease - .75f) / .25f);
			}
			else if (ease < .25f) {
				alpha = ease / .25f;
			}
			else {
				alpha = 1.f;
			}

		}
		break;
	default:
		alpha = 1.f;
		break;
	}

	// color switch with alpha
	if (alpha == 0.f) {
		m_color = Color::Transparent;
	}
	else {
		switch (m_type->m_colorMode) {
		case ParticleType::ColorModes::Static:
		case ParticleType::ColorModes::Choose:
			m_color = m_startColor;
			break;
		case ParticleType::ColorModes::Fade:
			m_color = Color::Lerp(m_type->m_color2, m_startColor, ease);
			break;
		case ParticleType::ColorModes::Blink:
			m_color = (Calc::BetweenInterval(m_life, 1.f) ? m_startColor : m_type->m_color2);
			break;
		}
		if (alpha < 1.f) {
			m_color *= alpha;
		}
	}

	// speed
	m_position += m_speed * dt;
	m_speed += m_type->m_acceleration * dt;
	m_speed = Calc::Approach(m_speed, Vec2_Zero, m_type->m_friction * dt);
	if (m_type->m_speedMultiplier != 1.f) {
		m_speed *= (float)Math_Pow(m_type->m_speedMultiplier, dt);
	}

	// scale out
	if (m_type->m_scaleOut) {
		m_size = m_startSize * Ease::CubeOut(ease);
	}
}

void Particle::Render()
{
	Math::Vec2 renderAt = m_position;
	if (m_track != nullptr) {
		renderAt += m_track->Position2D();
	}
	Draw::GetSpriteBatch()->Draw(m_source->Texture(), renderAt, m_source->ClipRect(), m_color, m_rotation, m_source->Center(), Math::Vec2(m_size), SpriteEffects::None, 0);
}
