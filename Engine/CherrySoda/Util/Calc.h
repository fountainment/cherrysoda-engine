#ifndef _CHERRYSODA_UTIL_CALC_H_
#define _CHERRYSODA_UTIL_CALC_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Random
{
public:
	// The default-constructed generator is deliberately deterministic so engine
	// runs stay reproducible; seed explicitly for randomized runs
	Random() = default;                       // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)
	Random(type::UInt32 seed) { Seed(seed); } // NOLINT(bugprone-random-generator-seed,cert-msc32-c,cert-msc51-cpp)

	inline void Seed(type::UInt32 seed) { STL::RandomSeed(m_random, seed); }
	inline type::UInt32 Next() { return STL::RandomNext(m_random); }
	inline type::UInt32 Next(type::UInt32 max) { return static_cast<type::UInt32>(max * NextDouble()) % max; }
	inline type::UInt32 Next(type::UInt32 min, type::UInt32 max) { return min + Next(max - min + 1); }
	inline type::Int32 NextInt(type::Int32 max) { return static_cast<type::Int32>(max * NextDouble()) % max; }
	inline type::Int32 NextInt(type::Int32 min, type::Int32 max) { return min + NextInt(max - min + 1); }
	inline float NextFloat() { return static_cast<float>(NextDouble()); }
	inline float NextFloat(float max) { return NextFloat() * max; }
	inline float NextFloat(float min, float max) { return min + NextFloat(max - min); }
	inline double NextDouble() { return Next() / static_cast<double>(STL::RandomMax(m_random)); }
	inline float NextDouble(double max) { return NextDouble() * max; }
	inline float NextDouble(double min, double max) { return min + NextDouble(max - min); }
	inline float NextAngle() { return NextFloat(Math::Pi2); }
	inline Math::Vec2 ShakeVector()
	{
		constexpr float shakeVectorOffsets[5] = {-1.f, -1.f, 0.f, 1.f, 1.f};
		return {shakeVectorOffsets[Next(5)], shakeVectorOffsets[Next(5)]};
	}

	template<typename T> inline T Choose(const T& a, const T& b) { return Next(2) ? a : b; }
	template<typename T> inline T Choose(const STL::Vector<T>& choices)
	{
		return choices[Next(static_cast<type::UInt32>(STL::Count(choices)))];
	}

	inline Math::Vec2 Range(const Math::Vec2& min, const Math::Vec2& max)
	{
		return min + Math::Vec2(NextFloat(max.x - min.x), NextFloat(max.y - min.y));
	}
	inline Math::Vec3 Range(const Math::Vec3& min, const Math::Vec3& max)
	{
		return min + Math::Vec3(NextFloat(max.x - min.x), NextFloat(max.y - min.y), NextFloat(max.z - min.z));
	}

	static type::UInt32 TrueRandomNext();

private:
	STL::Random m_random;
};

class Calc
{
public:
	// Random
	static void PushRandom(type::UInt32 seed) { STL::Push(ms_randomStack, Random(seed)); }
	static void PushRandom() { STL::Push(ms_randomStack, Random()); }
	static void PopRandom() { STL::Pop(ms_randomStack); }
	static Random* GetRandom() { return &STL::TopRef(ms_randomStack); }

	static inline float Angle(const Math::Vec2& vec) { return Math_Atan2(vec.y, vec.x); }
	static inline Math::Vec2 AngleToVector(float angleRadians, float length)
	{
		return {Math_Cos(angleRadians) * length, Math_Sin(angleRadians) * length};
	}

	static inline float Approach(float val, float target, float maxMove)
	{
		return val > target ? Math_Max(val - maxMove, target) : Math_Min(val + maxMove, target);
	}

	static Math::Vec2 Approach(const Math::Vec2& val, const Math::Vec2& target, float maxMove);
	static Math::Vec3 Approach(const Math::Vec3& val, const Math::Vec3& target, float maxMove);

	// Maps a value in [0, 1] onto a return trip 0 -> 1 -> 0
	static inline float YoYo(float value) { return value <= .5f ? value * 2.f : 1.f - ((value - .5f) * 2.f); }

	static inline float Map(float val, float min, float max, float newMin = 0.f, float newMax = 1.f)
	{
		return ((val - min) / (max - min) * (newMax - newMin)) + newMin;
	}

	static inline float ClampedMap(float val, float min, float max, float newMin = 0.f, float newMax = 1.f)
	{
		return (Math_Clamp((val - min) / (max - min), 0.f, 1.f) * (newMax - newMin)) + newMin;
	}

	static Math::Vec2 FourWayNormal(Math::Vec2 vec);
	static Math::Vec2 EightWayNormal(Math::Vec2 vec);
	static Math::Vec2 SnapedNormal(Math::Vec2 vec, float slices);
	static Math::Vec2 Snapped(Math::Vec2 vec, float slices);

	static inline int SignThreshold(float value, float threshold)
	{
		int sign = value > 0.f ? 1 : (value < 0.f ? -1 : 0);
		return sign * (Math_Abs(value) >= threshold ? 1 : 0);
	}

	static inline Math::Vec2 SafeNormalize(const Math::Vec2& vec, const Math::Vec2& ifZero)
	{
		return (vec == Vec2_Zero) ? ifZero : Math_Normalize(vec);
	}
	static inline Math::Vec2 SafeNormalize(const Math::Vec2& vec) { return SafeNormalize(vec, Vec2_Zero); }

	static Math::Vec2 ClosestPointOnLine(const Math::Vec2& lineA, const Math::Vec2& lineB, const Math::Vec2& closestTo);

	static inline Math::Vec2 Perpendicular(const Math::Vec2& vec) { return {vec.y, -vec.x}; }

	// Angles
	static inline float ReflectAngle(float angle, float axis = 0.f) { return -(angle + axis) - axis; }
	static inline float ReflectAngle(float angleRadians, const Math::Vec2& axis)
	{
		return ReflectAngle(angleRadians, Angle(axis));
	}

	static inline float WrapAngleDeg(float angleDegrees)
	{
		float sign = angleDegrees > 0.f ? 1.f : (angleDegrees < 0.f ? -1.f : 0.f);
		return (std::fmod((angleDegrees * sign) + 180.f, 360.f) - 180.f) * sign;
	}

	static inline float WrapAngle(float angleRadians)
	{
		float sign = angleRadians > 0.f ? 1.f : (angleRadians < 0.f ? -1.f : 0.f);
		return (std::fmod((angleRadians * sign) + Math::Pi, Math::Pi2) - Math::Pi) * sign;
	}

	static inline float AngleDiff(float radiansA, float radiansB)
	{
		float diff = radiansB - radiansA;
		while (diff > Math::Pi) {
			diff -= Math::Pi2;
		}
		while (diff <= -Math::Pi) {
			diff += Math::Pi2;
		}
		return diff;
	}

	static inline float AbsAngleDiff(float radiansA, float radiansB) { return Math_Abs(AngleDiff(radiansA, radiansB)); }

	static inline int SignAngleDiff(float radiansA, float radiansB)
	{
		float diff = AngleDiff(radiansA, radiansB);
		return diff > 0.f ? 1 : (diff < 0.f ? -1 : 0);
	}

	static inline float AngleApproach(float val, float target, float maxMove)
	{
		float diff = AngleDiff(val, target);
		if (Math_Abs(diff) < maxMove) {
			return target;
		}
		return val + Math_Clamp(diff, -maxMove, maxMove);
	}

	static inline float AngleLerp(float startAngle, float endAngle, float percent)
	{
		return startAngle + (AngleDiff(startAngle, endAngle) * percent);
	}

	static inline float ShorterAngleDifference(float currentAngle, float angleA, float angleB)
	{
		return Math_Abs(AngleDiff(currentAngle, angleA)) < Math_Abs(AngleDiff(currentAngle, angleB)) ? angleA : angleB;
	}

	static inline float ShorterAngleDifference(float currentAngle, float angleA, float angleB, float angleC)
	{
		// the recursive calls narrow the candidate set on purpose
		// NOLINTBEGIN(readability-suspicious-call-argument)
		return Math_Abs(AngleDiff(currentAngle, angleA)) < Math_Abs(AngleDiff(currentAngle, angleB))
				   ? ShorterAngleDifference(currentAngle, angleA, angleC)
				   : ShorterAngleDifference(currentAngle, angleB, angleC);
		// NOLINTEND(readability-suspicious-call-argument)
	}

	static inline bool BetweenInterval(float val, float interval) { return Math_Mod(val, interval * 2.f) >= interval; }

	static inline bool OnInterval(float val, float prevVal, float interval)
	{
		return static_cast<int>(prevVal / interval) != static_cast<int>(val / interval);
	}

	static inline int Digits(int num)
	{
		int digits = 1;
		int target = 10;
		while (num >= target) {
			++digits;
			target *= 10;
		}
		return digits;
	}

	// Save and Load Data
	static bool FileExists(const String& filename);

private:
	static STL::Stack<Random> ms_randomStack;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CALC_H_
