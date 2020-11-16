#ifndef _CHERRYSODA_UTIL_EASE_H_
#define _CHERRYSODA_UTIL_EASE_H_

#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

typedef STL::Func<float,float> Easer;

class Ease
{
public:
	static const Easer Linear;

	static const Easer SineIn;
	static const Easer SineOut;
	static const Easer SineInOut;

	static const Easer QuadIn;
	static const Easer QuadOut;
	static const Easer QuadInOut;

	static const Easer CubeIn;
	static const Easer CubeOut;
	static const Easer CubeInOut;

	static const Easer QuintIn;
	static const Easer QuintOut;
	static const Easer QuintInOut;

	static const Easer ExpoIn;
	static const Easer ExpoOut;
	static const Easer ExpoInOut;

	static const Easer BackIn;
	static const Easer BackOut;
	static const Easer BackInOut;

	static const Easer BigBackIn;
	static const Easer BigBackOut;
	static const Easer BigBackInOut;

	static const Easer ElasticIn;
	static const Easer ElasticOut;
	static const Easer ElasticInOut;

	static const Easer BounceIn;
	static const Easer BounceOut;
	static const Easer BounceInOut;

	static inline Easer Invert(Easer easer)
	{
		return [easer](float t){ return 1.f - easer(1.f - t); };
	}

	static inline Easer Follow(Easer first, Easer second)
	{
		return [first, second](float t){ return (t <= 0.5f) ? first(t * 2.f) * 0.5f : second(t * 2.f - 1.f) * 0.5f + 0.5f; };
	}

	static inline float UpDown(float eased)
	{
		return (eased <= 0.5f) ? eased * 2.f : 1.f - (eased - .5f) * 2.f; 
	}
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_EASE_H_
