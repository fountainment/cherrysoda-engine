#ifndef _CHERRYSODA_LOG_H_
#define _CHERRYSODA_LOG_H_

#include <CherrySoda/Utility/String.h>

#ifdef NDEBUG
#define CherrySodaDebug(output)
#define CherrySodaDebug(output,channel)
#else
#define CherrySodaDebug(output)                cherrysoda::Log::DebugOutput(output)
#define CherrySodaDebugChannel(output,channel) cherrysoda::Log::DebugOutput(output,channel)
#endif

#define CherrySodaLog(output)                  cherrysoda::Log::LogOutput(output)
#define CherrySodaLogChannel(output,channel)   cherrysoda::Log::LogOutput(output,channel)

namespace cherrysoda {

class Log
{
public:
	static void DebugOutput(const String& output, int channel = 0);
	static void LogOutput(const String& output, int channel = 0);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_LOG_H_
