#include <CherrySoda/Components/Logic/SineWave.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Calc.h>

namespace cherrysoda {

void SineWave::Counter(float counter)
{
	m_counter = Math_Mod(counter + Math::Pi2 * 4.f, Math::Pi2 * 4.f);
	m_value = Math_Sin(m_counter);
	m_valueOverTwo = Math_Sin(m_counter / 2.f);
	m_twoValue = Math_Sin(m_counter * 2.f);
}

SineWave* SineWave::Randomize()
{
	Counter(Calc::GetRandom()->NextFloat() * Math::Pi2 * 2.f);
	return this;
}

void SineWave::Update()
{
	Counter(Counter() + Math::Pi2 * Frequency() * Rate() *
							(UseRawDeltaTime() ? Engine::Instance()->RawDeltaTime() : Engine::Instance()->DeltaTime()));
	if (m_onUpdate != nullptr) {
		m_onUpdate(Value());
	}
}

} // namespace cherrysoda
