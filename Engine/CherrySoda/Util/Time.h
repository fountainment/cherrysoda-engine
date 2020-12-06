#ifndef _CHERRYSODA_UTIL_TIME_H_
#define _CHERRYSODA_UTIL_TIME_H_

namespace cherrysoda {

	class Time
	{
	public:
		static double GetSystemTime();
		static void SleepForMilliseconds(unsigned int ms);
	};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_TIME_H_
