#include <CherrySoda/Util/Ease.h>

#include <CherrySoda/Util/Math.h>

using cherrysoda::Ease;

using cherrysoda::Easer;
using cherrysoda::Math;

const Easer Ease::Linear = [](float t){ return t; };

const Easer Ease::SineIn = [](float t){ return -static_cast<float>(Math_Cos(Math::PiHalf * t)) + 1.f; };
const Easer Ease::SineOut = [](float t){ return static_cast<float>(Math_Sin(Math::PiHalf * t)); };
const Easer Ease::SineInOut = [](float t){ return -static_cast<float>(Math_Cos(Math::Pi * t)) * .5f + .5f; };

const Easer Ease::QuadIn = [](float t){ return t * t; };
const Easer Ease::QuadOut = Invert(QuadIn);
const Easer Ease::QuadInOut = Follow(QuadIn, QuadOut);

const Easer Ease::CubeIn = [](float t){ return t * t * t; };
const Easer Ease::CubeOut = Invert(CubeIn);
const Easer Ease::CubeInOut = Follow(CubeIn, CubeOut);

const Easer Ease::QuintIn = [](float t){ return t * t * t * t * t; };
const Easer Ease::QuintOut = Invert(QuintIn);
const Easer Ease::QuintInOut = Follow(QuintIn, QuintOut);

const Easer Ease::ExpoIn = [](float t){ return static_cast<float>(Math_Pow(2.f, 10.f * (t - 1.f))); };
const Easer Ease::ExpoOut = Invert(ExpoIn);
const Easer Ease::ExpoInOut = Follow(ExpoIn, ExpoOut);

const Easer Ease::BackIn = [](float t){ return t * t * (2.70158f * t - 1.70158f); };
const Easer Ease::BackOut = Invert(BackIn);
const Easer Ease::BackInOut = Follow(BackIn, BackOut);

const Easer Ease::BigBackIn = [](float t){ return t * t * (4.f * t - 3.f); };
const Easer Ease::BigBackOut = Invert(BigBackIn);
const Easer Ease::BigBackInOut = Follow(BigBackIn, BigBackOut);

const Easer Ease::ElasticIn = [](float t)
	{
		float ts = t * t;
		float tc = ts * t;
		return (33 * tc * ts + -59 * ts * ts + 32 * tc + -5 * ts);
	};

const Easer Ease::ElasticOut = [](float t)
	{
		float ts = t * t;
		float tc = ts * t;
		return (33 * tc * ts + -106 * ts * ts + 126 * tc + -67 * ts + 15 * t);
	};

const Easer Ease::ElasticInOut = Follow(ElasticIn, ElasticOut);

constexpr float B1 = 1.f / 2.75f;
constexpr float B2 = 2.f / 2.75f;
constexpr float B3 = 1.5f / 2.75f;
constexpr float B4 = 2.5f / 2.75f;
constexpr float B5 = 2.25f / 2.75f;
constexpr float B6 = 2.625f / 2.75f;

const Easer Ease::BounceIn = [](float t)
	{
		t = 1 - t;
		if (t < B1)
			return 1 - 7.5625f * t * t;
		if (t < B2)
			return 1 - (7.5625f * (t - B3) * (t - B3) + .75f);
		if (t < B4)
			return 1 - (7.5625f * (t - B5) * (t - B5) + .9375f);
		return 1 - (7.5625f * (t - B6) * (t - B6) + .984375f);
	};

const Easer Ease::BounceOut = [](float t)
	{
		if (t < B1)
			return 7.5625f * t * t;
		if (t < B2)
			return 7.5625f * (t - B3) * (t - B3) + .75f;
		if (t < B4)
			return 7.5625f * (t - B5) * (t - B5) + .9375f;
		return 7.5625f * (t - B6) * (t - B6) + .984375f;
	};

const Easer Ease::BounceInOut = [](float t)
	{
		if (t < .5f) {
			t = 1 - t * 2;
			if (t < B1)
				return (1 - 7.5625f * t * t) / 2;
			if (t < B2)
				return (1 - (7.5625f * (t - B3) * (t - B3) + .75f)) / 2;
			if (t < B4)
				return (1 - (7.5625f * (t - B5) * (t - B5) + .9375f)) / 2;
			return (1 - (7.5625f * (t - B6) * (t - B6) + .984375f)) / 2;
		}
		t = t * 2 - 1;
		if (t < B1)
			return (7.5625f * t * t) / 2 + .5f;
		if (t < B2)
			return (7.5625f * (t - B3) * (t - B3) + .75f) / 2 + .5f;
		if (t < B4)
			return (7.5625f * (t - B5) * (t - B5) + .9375f) / 2 + .5f;
		return (7.5625f * (t - B6) * (t - B6) + .984375f) / 2 + .5f;
	};
