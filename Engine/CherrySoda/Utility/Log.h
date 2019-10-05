#ifndef _CHERRYSODA_LOG_H_
#define _CHERRYSODA_LOG_H_

#include <CherrySoda/Utility/String.h>

namespace cherrysoda {

class Log
{
public:
	static void DebugOutput(const String& output);
	static void LogOutput(const String& output);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_LOG_H_
