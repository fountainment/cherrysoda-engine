#include <CherrySoda/Util/Log.h>

#include <CherrySoda/Util/String.h>

#include <cstdio>

#ifdef _WIN32
	#include <Windows.h>
#endif

using cherrysoda::Log;

using cherrysoda::String;

void Log::DebugOutput(const String& output, int channel)
{
#if defined(_WIN32) && defined(_MSC_VER)
	OutputDebugString(output.c_str());
#else
	std::printf(output.c_str());
#endif
}

void Log::LogOutput(const String& output, int channel)
{
	std::printf(output.c_str());
}