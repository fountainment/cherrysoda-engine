#include <CherrySoda/Util/Time.h>

#include <SDL.h>

namespace cherrysoda {

double Time::GetSystemTime()
{
	return static_cast<double>(SDL_GetPerformanceCounter()) / static_cast<double>(SDL_GetPerformanceFrequency());
}

void Time::SleepForMilliseconds(unsigned int ms)
{
	SDL_Delay(ms);
}

} // namespace cherrysoda
