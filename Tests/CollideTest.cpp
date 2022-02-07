#include "Test.h"

namespace {

TEST(CollideTestLineCheck, Normal)
{
	Math::Vec2 intersection;
	EXPECT_TRUE(Collide::LineCheck(Math::Vec2(0.f,0.f),Math::Vec2(1.f,1.f),Math::Vec2(0.f,1.f),Math::Vec2(1.f,0.f)));
	Collide::LineCheck(Math::Vec2(0.f,0.f),Math::Vec2(1.f,1.f),Math::Vec2(0.f,1.f),Math::Vec2(1.f,0.f),intersection);
	EXPECT_EQ(intersection,Math::Vec2(0.5f,0.5f));

	EXPECT_FALSE(Collide::LineCheck(Math::Vec2(0.f,0.f),Math::Vec2(1.f,1.f),Math::Vec2(0.f,1.f),Math::Vec2(1.f,2.f)));

	EXPECT_TRUE(Collide::LineCheck(Math::Vec2(0.f,0.f),Math::Vec2(10.f,0.f),Math::Vec2(9.f,-1.f),Math::Vec2(9.f,9.f)));
	Collide::LineCheck(Math::Vec2(0.f,0.f),Math::Vec2(10.f,0.f),Math::Vec2(9.f,-1.f),Math::Vec2(9.f,9.f),intersection);
	EXPECT_EQ(intersection,Math::Vec2(9.f,0.f));

	EXPECT_TRUE(Collide::LineCheck(Math::Vec2(9.f,-1.f),Math::Vec2(9.f,9.f),Math::Vec2(0.f,0.f),Math::Vec2(10.f,0.f)));
	Collide::LineCheck(Math::Vec2(9.f,-1.f),Math::Vec2(9.f,9.f),Math::Vec2(0.f,0.f),Math::Vec2(10.f,0.f),intersection);
	EXPECT_EQ(intersection,Math::Vec2(9.f,0.f));
}

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

TEST(CollideTestRectToCircle, Normal)
{
	// Center
	EXPECT_TRUE(Collide::RectToCircle(7.f,9.f,2.f,3.f,Math::Vec2(8.f,10.f),0.5f));
	EXPECT_TRUE(Collide::RectToCircle(7.f,9.f,2.f,3.f,Math::Vec2(6.f,8.f),5.f));

	// Right
	EXPECT_TRUE(Collide::RectToCircle(0.f,0.f,1.f,10.f,Math::Vec2(5.f,5.f),4.01f));
	EXPECT_FALSE(Collide::RectToCircle(0.f,0.f,1.f,10.f,Math::Vec2(5.f,5.f),3.99f));
	EXPECT_TRUE(Collide::RectToCircle(29.f,10.f,1.f,10.f,Math::Vec2(34.f,15.f),4.01f));
	EXPECT_FALSE(Collide::RectToCircle(29.f,10.f,1.f,10.f,Math::Vec2(34.f,15.f),3.99f));

	// Left
	EXPECT_TRUE(Collide::RectToCircle(0.f,0.f,1.f,10.f,Math::Vec2(-4.f,5.f),4.01f));
	EXPECT_FALSE(Collide::RectToCircle(0.f,0.f,1.f,10.f,Math::Vec2(-4.f,5.f),3.99f));
	EXPECT_TRUE(Collide::RectToCircle(-10.f,-19.f,1.f,10.f,Math::Vec2(-14.f,-14.f),4.01f));
	EXPECT_FALSE(Collide::RectToCircle(-10.f,-19.f,1.f,10.f,Math::Vec2(-14.f,-14.f),3.99f));

	// Top
	EXPECT_TRUE(Collide::RectToCircle(0.f,0.f,2.f,10.f,Math::Vec2(1.f,11.f),1.01f));
	EXPECT_FALSE(Collide::RectToCircle(0.f,0.f,2.f,10.f,Math::Vec2(1.f,11.f),0.99f));
	EXPECT_TRUE(Collide::RectToCircle(-1.f,-11.f,2.f,10.f,Math::Vec2(0.f,0.f),1.01f));
	EXPECT_FALSE(Collide::RectToCircle(-1.f,-11.f,2.f,10.f,Math::Vec2(0.f,0.f),0.99f));

	// Bottom
	EXPECT_TRUE(Collide::RectToCircle(0.f,0.f,2.f,10.f,Math::Vec2(1.f,-1.f),1.01f));
	EXPECT_FALSE(Collide::RectToCircle(0.f,0.f,2.f,10.f,Math::Vec2(1.f,-1.f),0.99f));
	EXPECT_TRUE(Collide::RectToCircle(-5.f,6.f,2.f,10.f,Math::Vec2(-4.f,5.f),1.01f));
	EXPECT_FALSE(Collide::RectToCircle(-5.f,6.f,2.f,10.f,Math::Vec2(-4.f,5.f),0.99f));

	// RightTop
	EXPECT_TRUE(Collide::RectToCircle(-4.f,-3.f,2.f,1.f,Math::Vec2(0.f,0.f),2.9f));
	EXPECT_FALSE(Collide::RectToCircle(-4.f,-3.f,2.f,1.f,Math::Vec2(0.f,0.f),2.8f));

	// LeftTop
	EXPECT_TRUE(Collide::RectToCircle(2.f,-3.f,2.f,1.f,Math::Vec2(0.f,0.f),2.9f));
	EXPECT_FALSE(Collide::RectToCircle(2.f,-3.f,2.f,1.f,Math::Vec2(0.f,0.f),2.8f));

	// LeftBottom
	EXPECT_TRUE(Collide::RectToCircle(2.f,2.f,2.f,1.f,Math::Vec2(0.f,0.f),2.9f));
	EXPECT_FALSE(Collide::RectToCircle(2.f,2.f,2.f,1.f,Math::Vec2(0.f,0.f),2.8f));

	// RightBottom
	EXPECT_TRUE(Collide::RectToCircle(-4.f,2.f,2.f,1.f,Math::Vec2(0.f,0.f),2.9f));
	EXPECT_FALSE(Collide::RectToCircle(-4.f,2.f,2.f,1.f,Math::Vec2(0.f,0.f),2.8f));
}

TEST(CollideTestRectToLine, Normal)
{
	EXPECT_TRUE(Collide::RectToLine(0.f,0.f,1.f,10.f,Math::Vec2(6.f,5.f),Math::Vec2(0.f,10.01f)));
	EXPECT_FALSE(Collide::RectToLine(0.f,0.f,1.f,10.f,Math::Vec2(6.f,5.f),Math::Vec2(0.f,11.01f)));

	EXPECT_TRUE(Collide::RectToLine(0.f,0.f,1.f,10.f,Math::Vec2(6.f,5.f),Math::Vec2(0.99f,5.f)));
	EXPECT_FALSE(Collide::RectToLine(0.f,0.f,1.f,10.f,Math::Vec2(6.f,5.f),Math::Vec2(1.01f,5.f)));
}

TEST(CollideTestGetSector, Normal)
{
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(5.f,-1.f)),PointSectors::Bottom);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(5.f,11.f)),PointSectors::Top);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(-1.f,5.f)),PointSectors::Left);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(11.f,5.f)),PointSectors::Right);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(4.f,5.f)),PointSectors::Center);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(-1.f,-1.f)),PointSectors::BottomLeft);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(11.f,-1.f)),PointSectors::BottomRight);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(-1.f,11.f)),PointSectors::TopLeft);
	EXPECT_EQ(Collide::GetSector(0.f,0.f,10.f,10.f,Math::Vec2(11.f,11.f)),PointSectors::TopRight);

	EXPECT_EQ(Collide::GetSector(1.f,2.f,2.f,5.f,Math::Vec2(3.f,7.f)),PointSectors::TopRight);
	EXPECT_EQ(Collide::GetSector(1.f,2.f,2.f,5.f,Math::Vec2(1.f,2.f)),PointSectors::Center);
	EXPECT_EQ(Collide::GetSector(1.f,2.f,2.f,5.f,Math::Vec2(0.99f,1.99f)),PointSectors::BottomLeft);
}

} // namespace
