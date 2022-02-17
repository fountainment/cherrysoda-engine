#ifndef _CHERRYSODA_COMPONENTS_LOGIC_COUNTERSET_H_
#define _CHERRYSODA_COMPONENTS_LOGIC_COUNTERSET_H_

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <class T>
class CounterSet : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CounterSet<T>, Component);

	CounterSet()
		: base(true, false)
	{}

	float Get(const T& index)
	{
		float value;
		if (STL::TryGetValue(m_counters, index, value))
			return Math_Max(value - m_timer, 0.f);
		else
			return 0.f;
	}

	void Set(const T& index, float value)
	{
		m_counters[index] = m_timer + value;	
	}

	bool Check(const T& index)
	{
		float value;
		if (STL::TryGetValue(m_counters, index, value))
			return value - m_timer > 0.f;
		else
			return false;
	}

	void Update() override
	{
		m_timer += Engine::Instance()->DeltaTime();
	}

private:
	STL::HashMap<T, float> m_counters;
	float m_timer;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_LOGIC_COUNTERSET_H_
