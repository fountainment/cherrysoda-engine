#include <CherrySoda/Util/Time.h>

#include <SDL2/SDL.h>

using cherrysoda::Time;

double Time::GetSystemTime()
{
	return static_cast<double>(SDL_GetPerformanceCounter()) / static_cast<double>(SDL_GetPerformanceFrequency());
}
