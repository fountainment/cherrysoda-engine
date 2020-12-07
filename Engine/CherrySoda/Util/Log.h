#ifndef _CHERRYSODA_UTIL_LOG_H_
#define _CHERRYSODA_UTIL_LOG_H_

#include <string>
#include <cassert>

#ifdef CHERRYSODA_ENABLE_DEBUG
#	define CHERRYSODA_DEBUG(output)                 cherrysoda::Log::DebugOutput(output)
#	define CHERRYSODA_DEBUG_CHANNEL(output,channel) cherrysoda::Log::DebugOutput(output,channel)
#	define CHERRYSODA_ASSERT(condition,output)      if(!(condition)){CHERRYSODA_DEBUG(output);assert(0);}
#else
#	define CHERRYSODA_DEBUG(output)
#	define CHERRYSODA_DEBUG_CHANNEL(output,channel)
#	define CHERRYSODA_ASSERT(condition,output)
#endif

#define CHERRYSODA_LOG(output)                   cherrysoda::Log::LogOutput(output)
#define CHERRYSODA_LOG_CHANNEL(output,channel)   cherrysoda::Log::LogOutput(output,channel)

namespace cherrysoda {

typedef std::string String;

class Log
{
public:
	static void DebugOutput(const String& output, int channel = 0);
	static void LogOutput(const String& output, int channel = 0);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_LOG_H_
