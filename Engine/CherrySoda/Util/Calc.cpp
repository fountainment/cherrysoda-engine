#include <CherrySoda/Util/Calc.h>

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <fstream>

using cherrysoda::Calc;
using cherrysoda::Math;
using cherrysoda::Random;
using cherrysoda::STL;
using cherrysoda::String;

namespace type = cherrysoda::type;

STL::Stack<Random> Calc::ms_randomStack({ Random() });

type::UInt32 Random::TrueRandomNext()
{
	static STL::TrueRandom random;
	return STL::RandomNext(random);
}

Math::Vec2 Calc::Approach(const Math::Vec2& val, const Math::Vec2& target, float maxMove)
{
	if (maxMove <= 0.f || val == target) {
		return val;
	}
	Math::Vec2 diff = target - val;
	float lengthSq = Math_LengthSq(diff);
	if (lengthSq < maxMove * maxMove) {
		return target;
	}
	else {
		diff = Math_Normalize(diff);
		return val + diff * maxMove;
	}
}

Math::Vec2 Calc::FourWayNormal(Math::Vec2 vec)
{
	if (vec == Vec2_Zero)
		return Vec2_Zero;

	float angle = (float)Math_Floor((Calc::Angle(vec) + (float)Math::PiHalf * .5f) / Math::PiHalf) * Math::PiHalf;

	vec = AngleToVector(angle, 1.f);
	if (Math_Abs(vec.x) < .5f)
		vec.x = 0;
	else
		vec.x = Math_Sign(vec.x);

	if (Math_Abs(vec.y) < .5f)
		vec.y = 0;
	else
		vec.y = Math_Sign(vec.y);

	return vec;
}

Math::Vec2 Calc::EightWayNormal(Math::Vec2 vec)
{
	if (vec == Vec2_Zero)
		return Vec2_Zero;

	vec = AngleToVector((float)Math_Floor((Angle(vec) + (float)Math::PiQuarter * .5f) / ((float)Math::PiQuarter)) * ((float)Math::PiQuarter), 1.f);
	if (Math_Abs(vec.x) < .5f)
		vec.x = 0.f;
	else if (Math_Abs(vec.y) < .5f)
		vec.y = 0.f;
	return vec;
}

Math::Vec2 Calc::SnapedNormal(Math::Vec2 vec, float slices)
{
	float divider = Math::Pi2 / slices;

	float angle = (float)Math_Floor((Calc::Angle(vec) + divider * .5f) / divider) * divider;
	return AngleToVector(angle, 1.f);
}

Math::Vec2 Calc::ClosestPointOnLine(const Math::Vec2& lineA, const Math::Vec2& lineB, const Math::Vec2& closestTo)
{
	auto v = lineB - lineA;
	if (v == Vec2_Zero) return lineA;
	auto w = closestTo - lineA;
	float t = Math_Dot(w, v) / Math_Dot(v, v);
	t = Math_Clamp(t, 0.f, 1.f);

	return lineA + v * t;
}

bool Calc::FileExists(const String& filename)
{
	std::ifstream file(filename);
	if (file.is_open()) {
		file.close();
		return true;
	}
	else {
		return false;
	}
}
