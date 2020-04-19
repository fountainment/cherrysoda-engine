#ifndef _CHERRYSODA_UTIL_CHOOSER_H_
#define _CHERRYSODA_UTIL_CHOOSER_H_

#include <CherrySoda/Util/ChoiceSet.h>

namespace cherrysoda {

template <class T>
class Chooser
{
	typedef typename ChoiceSet<T>::Choice Choice;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CHOOSER_H_