#include <CherrySoda/Util/Calc.h>

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Calc;
using cherrysoda::Math;
using cherrysoda::Random;
using cherrysoda::STL;

namespace type = cherrysoda::type;

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

Math::Vec2 Calc::EightWayNormal(Math::Vec2 vec)
{
	if (vec == Vec2_Zero)
	{
		return Vec2_Zero;
	}
	vec = AngleToVector((float)Math_Floor((Angle(vec) + (float)Math::Pi / 8.f) / ((float)Math::Pi / 4.f)) * ((float)Math::Pi / 4.f), 1.f);
	if (Math_Abs(vec.x) < 0.5f)
	{
		vec.x = 0.f;
	}
	else if (Math_Abs(vec.y) < 0.5f)
	{
		vec.y = 0.f;
	}
	return vec;
}

Math::Vec2 Calc::ClosestPointToLine(const Math::Vec2& lineA, const Math::Vec2& lineB, const Math::Vec2& closestTo)
{
	auto v = lineB - lineA;
	if (v == Vec2_Zero) return lineA;
	auto w = closestTo - lineA;
	float t = Math_Dot(w, v) / Math_Dot(v, v);
	t = Math_Clamp(t, 0.f, 1.f);

	return lineA + v * t;
}

STL::Stack<Random> Calc::ms_randomStack({ Random() });
