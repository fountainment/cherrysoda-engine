#include <CherrySoda/Components/Logic/Tween.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Ease.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Tween;

using cherrysoda::Easer;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::STL;
using cherrysoda::TweenMode;

STL::Stack<Tween*> Tween::ms_cached;

Tween* Tween::Create(TweenMode mode, Easer easer/* = nullptr*/, float duration/* = 1.f*/, bool start/* = false*/)
{
	Tween* tween = nullptr;
	if (STL::IsEmpty(ms_cached)) {
		tween = new Tween();
	}
	else {
		tween = STL::Pop(ms_cached);
	}
	tween->m_onUpdate = (tween->m_onComplete = (tween->m_onStart = nullptr));
	tween->Init(mode, easer, duration, start);
	return tween;
}

void Tween::Init(TweenMode mode, Easer easer, float duration, bool start)
{
	if (duration <= 0)
	{
		duration = Math::epsf;
	}
	m_useRawDeltaTime = false;
	m_mode = mode;
	m_easer = easer;
	m_duration = duration;
	m_timeLeft = 0;
	m_percent = 0;
	Active(false);
	if (start)
	{
		Start();
	}
}

void Tween::Removed(Entity* entity)
{
	base::Removed(entity);	
	STL::Push(ms_cached, this);
}

void Tween::Update()
{
	m_timeLeft -= (m_useRawDeltaTime ? Engine::Instance()->RawDeltaTime() : Engine::Instance()->DeltaTime());
	m_percent = Math_Max(0.f, m_timeLeft) / m_duration;
	if (!m_reverse)
	{
		m_percent = 1.f - m_percent;
	}
	if (m_easer != nullptr)
	{
		m_eased = m_easer(m_percent);
	}
	else
	{
		m_eased = m_percent;
	}
	if (m_onUpdate != nullptr)
	{
		m_onUpdate(this);
	}
	if (!(m_timeLeft <= 0.f))
	{
		return;
	}
	m_timeLeft = 0.f;
	if (m_onComplete != nullptr)
	{
		m_onComplete(this);
	}
	switch (m_mode)
	{
	case TweenMode::Persist:
		Active(false);
		break;
	case TweenMode::Oneshot:
		Active(false);
		RemoveSelf();
		break;
	case TweenMode::Looping:
		Start(m_reverse);
		break;
	case TweenMode::YoyoOneshot:
		if (m_reverse == m_startedReversed)
		{
			Start(!m_reverse);
			m_startedReversed = !m_reverse;
		}
		else
		{
			Active(false);
			RemoveSelf();
		}
		break;
	case TweenMode::YoyoLooping:
		Start(!m_reverse);
		break;
	}
}

void Tween::Start(bool reverse/* = false*/)
{
	m_startedReversed = (m_reverse = reverse);
	m_timeLeft = m_duration;
	m_eased = (m_percent = (m_reverse ? 1 : 0));
	Active(true);
	if (m_onStart != nullptr)
	{
		m_onStart(this);
	}
}

void Tween::Stop()
{
	Active(false);
}

void Tween::Reset()
{
	m_timeLeft = m_duration;
	m_eased = (m_percent = (m_reverse ? 1 : 0));
}
