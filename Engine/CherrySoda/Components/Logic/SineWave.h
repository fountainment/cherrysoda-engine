#ifndef _CHERRYSODA_COMPONENTS_LOGIC_SINEWAVE_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_SINEWAVE_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

class SineWave : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(SineWave, Component);

	SineWave() : base(true, false) {}
	SineWave(float frequency) : base(true, false) { Frequency(frequency); }

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Frequency, m_frequency);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Rate, m_rate);
	CHERRYSODA_GETTER_SETTER_OF_BOOL(UseRawDeltaTime, m_useRawDeltaTime);
	CHERRYSODA_GETTER_SETTER_OF_TYPE(STL::Action<float>, OnUpdate, m_onUpdate);

	inline float Value() const { return m_value; }
	inline float ValueOverTwo() const { return m_valueOverTwo; }
	inline float TwoValue() const { return m_twoValue; }

	inline float ValueOffset(float offset) const { return Math_Sin(Counter() + offset); }

	SineWave* Randomize();
	void Reset() { Counter(0.f); }
	void StartUp() { Counter(Math::PiHalf); }
	void StartDown() { Counter(Math::PiHalf * 3.f); }

	float Counter() const { return m_counter; }
	void Counter(float counter);

	void Update() override;

private:
	float m_frequency = 1.f;
	float m_rate = 1.f;
	float m_counter = 0.f;
	float m_value = 0.f;
	float m_valueOverTwo = 0.f;
	float m_twoValue = 0.f;
	bool m_useRawDeltaTime = false;

	STL::Action<float> m_onUpdate = nullptr;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_SINEWAVE_H_
