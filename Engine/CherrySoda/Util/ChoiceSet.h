#ifndef _CHERRYSODA_UTIL_CHOICESET_H_
#define _CHERRYSODA_UTIL_CHOICESET_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

template <class T>
class ChoiceSet
{
public:
	struct Choice
	{
		Choice(const T& data, float weight)
			: m_data(data)
			, m_weight(weight)
		{}

		T m_data;
		float m_weight;
	};

private:
	int m_totalWeight;
	STL::HashMap<T,int> m_choices;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CHOICESET_H_