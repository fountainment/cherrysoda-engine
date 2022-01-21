#include "Test.h"

namespace {

TEST(CollideTestCircleToLine, Normal)
{
	EXPECT_FALSE(Collide::CircleToLine(Math::Vec2(5.0f,5.0f),5.f,Math::Vec2(0.0f),Math::Vec2(0.0f,10.f)));
	EXPECT_TRUE(Collide::CircleToLine(Math::Vec2(5.0f,5.0f),5.f,Math::Vec2(0.0f),Math::Vec2(0.01f,10.f)));
	EXPECT_TRUE(Collide::CircleToLine(Math::Vec2(0.5f,0.6f),0.1f,Math::Vec2(0.0f,0.2f),Math::Vec2(0.8f,1.0f)));
}

TEST(CollideTestCircleToLine, Extreme)
{
	// Radius of 1e20 or larger will cause problem, because its square in float is inf.
	EXPECT_TRUE(Collide::CircleToLine(Math::Vec2(1e18,1e18),1e19,Vec2_Zero,Vec2_One));

	// Radius of 1e-23 or smaller will cause problem, because its square in floar is zero.
	EXPECT_TRUE(Collide::CircleToLine(Vec2_Zero,1e-20,Vec2_Zero,Vec2_One));
	EXPECT_TRUE(Collide::CircleToLine(Vec2_Zero,1e-20,Vec2_One,Vec2_Zero));
	EXPECT_FALSE(Collide::CircleToLine(Vec2_Zero,1e-20,Math::Vec2(1e-20),Vec2_One));

	// Basically, 1e-9 ~ 1e9 will be safe
}

} // namespace
