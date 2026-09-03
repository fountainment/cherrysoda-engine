#include <CherrySoda/Util/Log.h>

#include <CherrySoda/Util/String.h>

#include <cstdio>

#ifdef _WIN32
	#include <Windows.h>
#endif

namespace cherrysoda {

void Log::DebugOutput(const String& output, int channel)
{
#if defined(_WIN32) && defined(_MSC_VER)
	OutputDebugString(output.c_str());
#else
	std::printf("%s", output.c_str());
#endif
}

void Log::LogOutput(const String& output, int channel)
{
	DebugOutput(output, channel);
	static FILE* s_logFile = std::fopen("cherrysoda.log", "a");
	if (s_logFile != nullptr) {
		std::fprintf(s_logFile, "%s", output.c_str());
		std::fflush(s_logFile);
	}
}

} // namespace cherrysoda
