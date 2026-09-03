#include "Test.h"

namespace {

TEST(EaseTestBoundary, StartAndEnd)
{
	// All of these easers hit exactly 0 at t=0 and 1 at t=1
	// (the Expo family is tested separately since ExpoIn(0) is not 0).
	const Easer easers[] = {
		Ease::Linear,     Ease::SineIn,       Ease::SineOut,    Ease::SineInOut,    Ease::QuadIn,
		Ease::QuadOut,    Ease::QuadInOut,    Ease::CubeIn,     Ease::CubeOut,      Ease::CubeInOut,
		Ease::QuintIn,    Ease::QuintOut,     Ease::QuintInOut, Ease::BackIn,       Ease::BackOut,
		Ease::BackInOut,  Ease::BigBackIn,    Ease::BigBackOut, Ease::BigBackInOut, Ease::ElasticIn,
		Ease::ElasticOut, Ease::ElasticInOut, Ease::BounceIn,   Ease::BounceOut,    Ease::BounceInOut,
	};

	for (const Easer& easer : easers) {
		EXPECT_FLOAT_EQ(0.f, easer(0.f));
		EXPECT_FLOAT_EQ(1.f, easer(1.f));
	}
}

TEST(EaseTestBoundary, ExpoSpecialValues)
{
	// ExpoIn has no special case for t=0, so it never quite reaches the boundaries.
	EXPECT_FLOAT_EQ(0.0009765625f, Ease::ExpoIn(0.f)); // 2^-10
	EXPECT_FLOAT_EQ(1.f, Ease::ExpoIn(1.f));
	EXPECT_FLOAT_EQ(0.f, Ease::ExpoOut(0.f));
	EXPECT_FLOAT_EQ(0.9990234375f, Ease::ExpoOut(1.f));    // 1 - 2^-10
	EXPECT_FLOAT_EQ(0.00048828125f, Ease::ExpoInOut(0.f)); // 2^-11
	EXPECT_FLOAT_EQ(0.99951171875f, Ease::ExpoInOut(1.f)); // 1 - 2^-11
}

TEST(EaseTestMidpoint, KnownValues)
{
	EXPECT_FLOAT_EQ(0.5f, Ease::Linear(0.5f));

	EXPECT_FLOAT_EQ(0.29289322f, Ease::SineIn(0.5f));
	EXPECT_FLOAT_EQ(0.70710678f, Ease::SineOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::SineInOut(0.5f));

	EXPECT_FLOAT_EQ(0.25f, Ease::QuadIn(0.5f));
	EXPECT_FLOAT_EQ(0.75f, Ease::QuadOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::QuadInOut(0.5f));

	EXPECT_FLOAT_EQ(0.125f, Ease::CubeIn(0.5f));
	EXPECT_FLOAT_EQ(0.875f, Ease::CubeOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::CubeInOut(0.5f));

	EXPECT_FLOAT_EQ(0.03125f, Ease::QuintIn(0.5f));
	EXPECT_FLOAT_EQ(0.96875f, Ease::QuintOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::QuintInOut(0.5f));

	EXPECT_FLOAT_EQ(0.03125f, Ease::ExpoIn(0.5f));  // 2^-5
	EXPECT_FLOAT_EQ(0.96875f, Ease::ExpoOut(0.5f)); // 1 - 2^-5

	EXPECT_NEAR(-0.0876975f, Ease::BackIn(0.5f), 1e-6f); // back overshoots below zero
	EXPECT_FLOAT_EQ(-0.25f, Ease::BigBackIn(0.5f));

	EXPECT_FLOAT_EQ(0.09375f, Ease::ElasticIn(0.5f));
	EXPECT_FLOAT_EQ(0.90625f, Ease::ElasticOut(0.5f));

	EXPECT_FLOAT_EQ(0.234375f, Ease::BounceIn(0.5f));
	EXPECT_FLOAT_EQ(0.765625f, Ease::BounceOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::BounceInOut(0.5f));
}

TEST(EaseTestInvert, OutIsInvertedIn)
{
	const float ts[] = {0.25f, 0.5f, 0.75f};

	for (float t : ts) {
		EXPECT_FLOAT_EQ(1.f - Ease::SineIn(1.f - t), Ease::SineOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::QuadIn(1.f - t), Ease::QuadOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::CubeIn(1.f - t), Ease::CubeOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::QuintIn(1.f - t), Ease::QuintOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::ExpoIn(1.f - t), Ease::ExpoOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::BackIn(1.f - t), Ease::BackOut(t));
		EXPECT_FLOAT_EQ(1.f - Ease::BigBackIn(1.f - t), Ease::BigBackOut(t));
	}

	// Invert builds the mirror of any easer
	Easer inverted = Ease::Invert(Ease::QuadIn);
	EXPECT_FLOAT_EQ(1.f - Ease::QuadIn(1.f - 0.3f), inverted(0.3f));
	EXPECT_FLOAT_EQ(Ease::QuadOut(0.8f), Ease::Invert(Ease::QuadIn)(0.8f));
}

TEST(EaseTestFollow, HalvesAndContinuity)
{
	// Any Follow crosses through exactly 0.5 at the midpoint
	EXPECT_FLOAT_EQ(0.5f, Ease::Follow(Ease::QuadIn, Ease::QuadOut)(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::Follow(Ease::CubeIn, Ease::BounceOut)(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::QuadInOut(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::ElasticInOut(0.5f));

	// First half of a Follow runs the first easer double-speed, halved
	EXPECT_FLOAT_EQ(Ease::QuadIn(0.5f) * 0.5f, Ease::QuadInOut(0.25f));
	EXPECT_FLOAT_EQ(Ease::QuadOut(0.5f) * 0.5f + 0.5f, Ease::QuadInOut(0.75f));
}

TEST(EaseTestUpDown, TriangleWave)
{
	EXPECT_FLOAT_EQ(0.f, Ease::UpDown(0.f));
	EXPECT_FLOAT_EQ(0.5f, Ease::UpDown(0.25f));
	EXPECT_FLOAT_EQ(1.f, Ease::UpDown(0.5f));
	EXPECT_FLOAT_EQ(0.5f, Ease::UpDown(0.75f));
	EXPECT_FLOAT_EQ(0.f, Ease::UpDown(1.f));
}

} // namespace
