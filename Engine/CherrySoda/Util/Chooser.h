#ifndef _CHERRYSODA_UTIL_CHOOSER_H_
#define _CHERRYSODA_UTIL_CHOOSER_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <class T>
class Chooser
{
private:
	class Choice
	{
	public:
		T m_value;
		float m_weight = 0.f;

		Choice() {}

		Choice(const T& value, float weight)
		: m_value(value)
		, m_weight(weight)
		{}
	};

public:
	Chooser() = default;

	Chooser(T firstChoice, float weight)
	{
		Add(firstChoice, weight);
	}

	Chooser<T>& Add(T choice, float weight)
	{
		weight = Math_Max(weight, 0.f);
		STL::Add(m_choices, Choice(choice, weight));
		m_totalWeight += weight;
		return *this;

	}

	const T& Choose()
	{
		if (m_totalWeight <= 0.f) {
			return m_defaultChoice.m_value;
		}
		else if (STL::Count(m_choices) == 1) {
			return m_choices[0].m_value;
		}

		// float roll = ;
		return m_defaultChoice.m_value;
	}

	inline bool IsEmpty() { return STL::Count(m_choices) == 0; }

	STL::Vector<Choice> m_choices;
	Choice m_defaultChoice;
	float m_totalWeight = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CHOOSER_H_