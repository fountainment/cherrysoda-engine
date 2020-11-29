#include <CherrySoda/Components/Logic/Wiggler.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Wiggler;

using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::STL;

Wiggler* Wiggler::Create(float duration, float frequency, STL::Action<float> onChange/* = nullptr*/, bool start/* = false*/, bool removeSelfOnFinish/* = false*/)
{
	Wiggler* wiggler = nullptr;
	if (STL::IsEmpty(ms_cache)) {
		wiggler = new Wiggler();
	}
	else {
		wiggler = STL::Pop(ms_cache);
	}
	wiggler->Init(duration, frequency, onChange, start, removeSelfOnFinish);
	return wiggler;
}

void Wiggler::Removed(Entity* entity)
{
	base::Removed(entity);
	STL::Push(ms_cache, this);
}

void Wiggler::Init(float duration, float frequency, STL::Action<float> onChange, bool start, bool removeSelfOnFinish)
{
	m_counter = m_sineCounter = 0.f;
	m_startZero = false;
	m_useRawDeltaTime = false;

	m_increment = 1.f / duration;
	m_sineAdd = Math::Pi2 * frequency;
	m_onChange = onChange;
	m_removeSelfOnFinish = removeSelfOnFinish;

	if (start) {
		Start();
	}
	else {
		Active(false);
	}
}

void Wiggler::Start()
{
	m_counter = 1.f;

	if (m_startZero) {
		m_sineCounter = Math::PiHalf;
		m_value = 0.f;
		if (m_onChange != nullptr) {
			m_onChange(m_value);
		}
	}
	else {
		m_sineCounter = 0.f;
		m_value = 1.f;
		if (m_onChange != nullptr) {
			m_onChange(m_value);
		}

	}

	Active(true);
}

void Wiggler::Start(float duration, float frequency)
{
	m_increment = 1.f / duration;
	m_sineAdd = Math::Pi2 * frequency;
	Start();
}

void Wiggler::StopAndClear()
{
	Stop();
	m_value = 0.f;	
}

void Wiggler::Update()
{
	if (m_useRawDeltaTime) {
		m_sineCounter += m_sineAdd * Engine::Instance()->RawDeltaTime();
		m_counter -= m_increment * Engine::Instance()->RawDeltaTime();
	}
	else {
		m_sineCounter += m_sineAdd * Engine::Instance()->DeltaTime();
		m_counter -= m_increment * Engine::Instance()->DeltaTime();
	}

	if (m_counter <= 0.f) {
		m_counter = 0.f;
		Active(false);	
		if (m_removeSelfOnFinish) {
			RemoveSelf();
		}
	}

	m_value = static_cast<float>(Math_Cos(m_sineCounter)) * m_counter;

	if (m_onChange != nullptr) {
		m_onChange(m_value);
	}
}

STL::Stack<Wiggler*> Wiggler::ms_cache;
