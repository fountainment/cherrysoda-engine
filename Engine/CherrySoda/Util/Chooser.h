#ifndef _CHERRYSODA_UTIL_CHOOSER_H_
#define _CHERRYSODA_UTIL_CHOOSER_H_

#include <CherrySoda/Util/Calc.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
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

		Choice() = default;

		Choice(const T& value, float weight)
		: m_value(value)
		, m_weight(weight)
		{}
	};

public:
	Chooser() = default;

	Chooser(const T& firstChoice, float weight)
	{
		Add(firstChoice, weight);
	}

	Chooser<T>& Add(const T& choice, float weight)
	{
		weight = Math_Max(weight, 0.f);
		STL::Add(m_choices, Choice(choice, weight));
		m_totalWeight += weight;
		return *this;

	}

	const T& Choose() const
	{
		if (m_totalWeight <= 0) {
			return m_defaultChoice.m_value;
		}
		else if (STL::Count(m_choices) == 1) {
			return m_choices[0].m_value;
		}

		float roll = Calc::GetRandom()->NextDouble() * m_totalWeight;
		float check = 0;
		for (int i = 0; i < static_cast<int>(STL::Count(m_choices)) - 1; ++i) {
			check += m_choices[i].m_weight;
			if (roll < check) {
				return m_choices[i].m_value;
			}
		}

		return m_choices[STL::Count(m_choices) - 1].m_value;
	}

	inline bool CanChoose() const { return m_totalWeight > 0; }

	inline bool IsEmpty() const { return STL::IsEmpty(m_choices); }

	static Chooser<T> FromString(const String& data)
	{
		Chooser<T> chooser;
		auto choices = StringUtil::Split(data, ',');

		if (STL::Count(choices) == 1 && StringUtil::IndexOf(choices[0], ':') == -1) {
			chooser.Add(static_cast<T>(choices[0]), 1.f);
			return chooser;
		}

		for (auto& choice : choices) {
			if (StringUtil::IndexOf(choice, ':') == -1) {
				chooser.Add(static_cast<T>(choice), 1.f);
			}
			else {
				auto parts = StringUtil::Split(choice, ':');
				auto key = StringUtil::Trim(parts[0]);
				auto weight = StringUtil::Trim(parts[1]);

				chooser.Add(static_cast<T>(key), StringUtil::ToFloat(weight));
			}
		}

		return chooser;
	}

private:
	STL::Vector<Choice> m_choices;
	Choice m_defaultChoice;
	float m_totalWeight = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CHOOSER_H_
