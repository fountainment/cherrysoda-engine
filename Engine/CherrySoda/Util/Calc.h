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
	Random() = default;
	Random(type::UInt32 seed) { Seed(seed); }

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
		constexpr float shakeVectorOffsets[5] = { -1.f, -1.f, 0.f, 1.f, 1.f };
		return Math::Vec2(shakeVectorOffsets[Next(5)], shakeVectorOffsets[Next(5)]);
	}

	template <typename T>
	inline T Choose(const T& a, const T& b) { return Next(2) ? a : b; }

	inline Math::Vec2 Range(const Math::Vec2& min, const Math::Vec2& max) { return min + Math::Vec2(NextFloat(max.x - min.x), NextFloat(max.y - min.y)); }
	inline Math::Vec3 Range(const Math::Vec3& min, const Math::Vec3& max) { return min + Math::Vec3(NextFloat(max.x - min.x), NextFloat(max.y - min.y), NextFloat(max.z - min.z)); }

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
		return Math::Vec2((float)Math_Cos(angleRadians) * length, (float)Math_Sin(angleRadians) * length);
	}

	static Math::Vec2 Approach(const Math::Vec2& val, const Math::Vec2& target, float maxMove);

	static Math::Vec2 FourWayNormal(Math::Vec2 vec);
	static Math::Vec2 EightWayNormal(Math::Vec2 vec);
	static Math::Vec2 SnapedNormal(Math::Vec2 vec, float slices);

	static inline Math::Vec2 SafeNormalize(const Math::Vec2& vec, const Math::Vec2& ifZero)
	{
		return (vec == Vec2_Zero) ? ifZero : Math_Normalize(vec);
	}
	static inline Math::Vec2 SafeNormalize(const Math::Vec2& vec) { return SafeNormalize(vec, Vec2_Zero); }

	static Math::Vec2 ClosestPointOnLine(const Math::Vec2& lineA, const Math::Vec2& lineB, const Math::Vec2& closestTo);

	static inline Math::Vec2 Perpendicular(const Math::Vec2& vec) { return Math::Vec2(vec.y, -vec.x); }

	static inline bool BetweenInterval(float val, float interval) { return Math_Mod(val, interval * 2.f) >= interval; }

	// Save and Load Data
	static bool FileExists(const String& filename);

private:
	static STL::Stack<Random> ms_randomStack;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CALC_H_
 