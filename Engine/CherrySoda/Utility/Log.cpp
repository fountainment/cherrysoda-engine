#include <CherrySoda/Utility/Log.h>

#include <CherrySoda/Utility/String.h>

#ifdef _WIN32
	#include <Windows.h>
#else
	#include <cstdio>
#endif

using cherrysoda::Log;

using cherrysoda::String;

void Log::DebugOutput(const String& output)
{
	std::printf(output.c_str());
}

void Log::LogOutput(const String& output)
{
	std::printf(output.c_str());
}
