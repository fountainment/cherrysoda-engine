#ifndef _CHERRYSODA_UTILITY_STRING_H_
#define _CHERRYSODA_UTILITY_STRING_H_

#include <string>

namespace cherrysoda {

typedef std::string String;

class StringUtil
{
public:
	static const String FormatString(const char* format, ...);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTILITY_STRING_H_