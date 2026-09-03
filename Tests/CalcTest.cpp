#include "Test.h"

namespace {

TEST(CalcTestClosestPointOnLine, Horizontal)
{
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(5.0f,0.0f),Math::Vec2(2.5f,2.5f),Math::Vec2(2.5f,0.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(8.5f,8.5f),Math::Vec2(8.5f,9.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(-100.5f,8.5f),Math::Vec2(7.0f,9.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(100.5f,8.5f),Math::Vec2(15.0f,9.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(10.5f,9.0f),Math::Vec2(10.5f,9.0f));
}

TEST(CalcTestClosestPointOnLine, Vertical)
{
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.0f,5.0f),Math::Vec2(2.5f,2.5f),Math::Vec2(0.0f,2.5f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,10.5f),Math::Vec2(9.5f,10.5f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,-100.5f),Math::Vec2(9.5f,8.5f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,100.5f),Math::Vec2(9.5f,12.5f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(9.5f,10.5f),Math::Vec2(9.5f,10.5f));
}

TEST(CalcTestClosestPointOnLine, Diagonal)
{
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,100.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(50.0f,50.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,50.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(80.0f,40.0f));
}

TEST(CalcTestClosestPointOnLine, Extreme)
{
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.000001f,0.0f),Math::Vec2(-100.0f,0.0f),Math::Vec2(0.0f,0.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.000001f,0.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(0.000001f,0.0f));
	TEST_CLOSESTPOINTONLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(0.0f,0.0f));
}

TEST(CalcTestFileExist, Normal)
{
	EXPECT_TRUE(Calc::FileExists("gamecontrollerdb.txt"));
	EXPECT_FALSE(Calc::FileExists("123.txt"));
}

TEST(CalcTestRandom, DeterministicWithSameSeed)
{
	Random a(42);
	Random b(42);
	Random c(43);

	for (int i = 0; i < 10; ++i) {
		type::UInt32 va = a.Next();
		EXPECT_EQ(va, b.Next());
		EXPECT_NE(va, c.Next());  // astronomically unlikely to match every time
	}

	Random a2(42);
	Random b2(42);
	for (int i = 0; i < 10; ++i) {
		EXPECT_EQ(a2.NextFloat(), b2.NextFloat());
	}
}

TEST(CalcTestRandom, ValueRanges)
{
	Random random(1);

	for (int i = 0; i < 1000; ++i) {
		EXPECT_GE(random.NextDouble(), 0.0);
		EXPECT_LT(random.NextDouble(), 1.0);

		EXPECT_GE(random.NextFloat(), 0.f);
		EXPECT_LT(random.NextFloat(), 1.f);

		type::UInt32 bounded = random.Next(10u);
		EXPECT_LT(bounded, 10u);

		type::UInt32 ranged = random.Next(3u, 7u);
		EXPECT_GE(ranged, 3u);
		EXPECT_LE(ranged, 7u);

		type::Int32 intBounded = random.NextInt(10);
		EXPECT_GE(intBounded, 0);
		EXPECT_LT(intBounded, 10);

		type::Int32 intRanged = random.NextInt(-5, 5);
		EXPECT_GE(intRanged, -5);
		EXPECT_LE(intRanged, 5);

		float floatRanged = random.NextFloat(1.5f, 2.5f);
		EXPECT_GE(floatRanged, 1.5f);
		EXPECT_LE(floatRanged, 2.5f);
	}
}

TEST(CalcTestRandom, RangeVecAndShakeVectorAndChoose)
{
	Random random(7);

	for (int i = 0; i < 100; ++i) {
		Math::Vec2 v = random.Range(Math::Vec2(1.f, 2.f), Math::Vec2(3.f, 4.f));
		EXPECT_GE(v.x, 1.f);
		EXPECT_LE(v.x, 3.f);
		EXPECT_GE(v.y, 2.f);
		EXPECT_LE(v.y, 4.f);

		Math::Vec3 v3 = random.Range(Math::Vec3(-1.f), Math::Vec3(1.f));
		EXPECT_GE(v3.x, -1.f);
		EXPECT_LE(v3.x, 1.f);
		EXPECT_GE(v3.y, -1.f);
		EXPECT_LE(v3.y, 1.f);
		EXPECT_GE(v3.z, -1.f);
		EXPECT_LE(v3.z, 1.f);

		Math::Vec2 shake = random.ShakeVector();
		EXPECT_TRUE(shake.x == -1.f || shake.x == 0.f || shake.x == 1.f);
		EXPECT_TRUE(shake.y == -1.f || shake.y == 0.f || shake.y == 1.f);

		int chosen = random.Choose(7, 9);
		EXPECT_TRUE(chosen == 7 || chosen == 9);
	}
}

TEST(CalcTestRandom, ChooseHitsBothSides)
{
	Random random(99);
	bool sawSeven = false;
	bool sawNine = false;
	for (int i = 0; i < 100; ++i) {
		int chosen = random.Choose(7, 9);
		sawSeven = sawSeven || chosen == 7;
		sawNine = sawNine || chosen == 9;
	}
	EXPECT_TRUE(sawSeven);
	EXPECT_TRUE(sawNine);
}

TEST(CalcTestRandomStack, PushAndPop)
{
	// Pushing a seeded Random makes GetRandom() deterministic; a local Random
	// seeded the same way produces the identical sequence.
	Calc::PushRandom(123);
	Random local(123);
	EXPECT_EQ(local.NextFloat(), Calc::GetRandom()->NextFloat());
	EXPECT_EQ(local.Next(), Calc::GetRandom()->Next());
	Calc::PopRandom();

	// The same seed produces the same sequence across pushes
	Calc::PushRandom(7);
	type::UInt32 first = Calc::GetRandom()->Next();
	Calc::PopRandom();

	Calc::PushRandom(7);
	EXPECT_EQ(first, Calc::GetRandom()->Next());
	Calc::PopRandom();
}

TEST(CalcTestApproach, MovesTowardTarget)
{
	EXPECT_EQ(Math::Vec2(3.f, 0.f), Calc::Approach(Math::Vec2(0.f), Math::Vec2(10.f, 0.f), 3.f));
	EXPECT_EQ(Math::Vec2(1.5f, 2.f), Calc::Approach(Math::Vec2(0.f), Math::Vec2(3.f, 4.f), 2.5f)); // 3-4-5 triangle
	EXPECT_EQ(Math::Vec2(3.f, 4.f), Calc::Approach(Math::Vec2(0.f), Math::Vec2(3.f, 4.f), 10.f)); // overshoot clamps to target
	EXPECT_EQ(Math::Vec2(2.f, 0.f), Calc::Approach(Math::Vec2(0.f), Math::Vec2(2.f, 0.f), 5.f));

	// Already at target or zero max move stays put
	EXPECT_EQ(Math::Vec2(5.f, 5.f), Calc::Approach(Math::Vec2(5.f, 5.f), Math::Vec2(5.f, 5.f), 3.f));
	EXPECT_EQ(Math::Vec2(1.f, 1.f), Calc::Approach(Math::Vec2(1.f, 1.f), Math::Vec2(9.f, 9.f), 0.f));
}

TEST(CalcTestNormals, FourWay)
{
	EXPECT_EQ(Math::Vec2(1.f, 0.f), Calc::FourWayNormal(Math::Vec2(1.f, 0.1f)));
	EXPECT_EQ(Math::Vec2(0.f, 1.f), Calc::FourWayNormal(Math::Vec2(1.f, 1.f)));    // diagonals snap up
	EXPECT_EQ(Math::Vec2(-1.f, 0.f), Calc::FourWayNormal(Math::Vec2(-3.f, 0.2f)));
	EXPECT_EQ(Math::Vec2(0.f, -1.f), Calc::FourWayNormal(Math::Vec2(0.f, -1.f)));
	EXPECT_EQ(Math::Vec2(0.f, 0.f), Calc::FourWayNormal(Vec2_Zero));
}

TEST(CalcTestNormals, EightWay)
{
	EXPECT_EQ(Math::Vec2(1.f, 0.f), Calc::EightWayNormal(Math::Vec2(10.f, 1.f)));

	// 45 degree diagonals stay diagonal
	Math::Vec2 diagonal = Calc::EightWayNormal(Math::Vec2(2.f, 1.f));
	EXPECT_NEAR(0.70710678f, diagonal.x, 1e-5f);
	EXPECT_NEAR(0.70710678f, diagonal.y, 1e-5f);

	EXPECT_EQ(Math::Vec2(0.f, 1.f), Calc::EightWayNormal(Math::Vec2(0.2f, 5.f)));
	EXPECT_EQ(Math::Vec2(0.f, 0.f), Calc::EightWayNormal(Vec2_Zero));
}

TEST(CalcTestNormals, Snaped)
{
	Math::Vec2 snapped = Calc::SnapedNormal(Math::Vec2(1.f, 0.5f), 8.f);
	EXPECT_NEAR(0.70710678f, snapped.x, 1e-5f);
	EXPECT_NEAR(0.70710678f, snapped.y, 1e-5f);

	// SnapedNormal keeps raw trig components (no sign cleanup like Four/EightWay)
	Math::Vec2 up = Calc::SnapedNormal(Math::Vec2(1.f, 1.f), 4.f);
	EXPECT_NEAR(0.f, up.x, 1e-5f);
	EXPECT_NEAR(1.f, up.y, 1e-5f);

	EXPECT_EQ(Math::Vec2(1.f, 0.f), Calc::SnapedNormal(Math::Vec2(0.9f, 0.1f), 4.f));
}

TEST(CalcTestAngle, AngleAndAngleToVector)
{
	EXPECT_NEAR(0.f, Calc::Angle(Math::Vec2(1.f, 0.f)), 1e-5f);
	EXPECT_NEAR(Math::PiHalf, Calc::Angle(Math::Vec2(0.f, 1.f)), 1e-5f);
	EXPECT_NEAR(-Math::PiHalf, Calc::Angle(Math::Vec2(0.f, -1.f)), 1e-5f);
	EXPECT_NEAR(Math::Pi * 0.75f, Calc::Angle(Math::Vec2(-1.f, 1.f)), 1e-5f);

	EXPECT_EQ(Math::Vec2(5.f, 0.f), Calc::AngleToVector(0.f, 5.f));
	EXPECT_NEAR(0.f, Calc::AngleToVector(Math::PiHalf, 2.f).x, 1e-5f);
	EXPECT_NEAR(2.f, Calc::AngleToVector(Math::PiHalf, 2.f).y, 1e-5f);

	// AngleToVector round-trips through Angle for arbitrary directions
	Math::Vec2 vector = Calc::AngleToVector(1.234f, 3.f);
	EXPECT_NEAR(1.234f, Calc::Angle(vector), 1e-4f);
	EXPECT_NEAR(3.f, Math_Length(vector), 1e-4f);
}

TEST(CalcTestVecUtils, SafeNormalizePerpendicularBetweenInterval)
{
	Math::Vec2 normalized = Calc::SafeNormalize(Math::Vec2(3.f, 4.f));
	EXPECT_FLOAT_EQ(0.6f, normalized.x);
	EXPECT_FLOAT_EQ(0.8f, normalized.y);

	// Zero vector falls back to the provided value
	EXPECT_EQ(Vec2_Zero, Calc::SafeNormalize(Vec2_Zero));
	EXPECT_EQ(Math::Vec2(1.f, 2.f), Calc::SafeNormalize(Vec2_Zero, Math::Vec2(1.f, 2.f)));

	EXPECT_EQ(Math::Vec2(4.f, -3.f), Calc::Perpendicular(Math::Vec2(3.f, 4.f)));

	EXPECT_FALSE(Calc::BetweenInterval(0.6f, 1.f));
	EXPECT_TRUE(Calc::BetweenInterval(1.5f, 1.f));
	EXPECT_FALSE(Calc::BetweenInterval(2.6f, 1.f)); // wraps back into the first half
}

} // namespace
