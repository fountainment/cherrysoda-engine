#include "Test.h"

namespace {

TEST(CalcTestClosestPointToLine, Horizontal)
{
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(5.0f,0.0f),Math::Vec2(2.5f,2.5f),Math::Vec2(2.5f,0.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(8.5f,8.5f),Math::Vec2(8.5f,9.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(-100.5f,8.5f),Math::Vec2(7.0f,9.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(100.5f,8.5f),Math::Vec2(15.0f,9.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(7.0f,9.0f),Math::Vec2(15.0f,9.0f),Math::Vec2(10.5f,9.0f),Math::Vec2(10.5f,9.0f));
}

TEST(CalcTestClosestPointToLine, Vertical)
{
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.0f,5.0f),Math::Vec2(2.5f,2.5f),Math::Vec2(0.0f,2.5f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,10.5f),Math::Vec2(9.5f,10.5f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,-100.5f),Math::Vec2(9.5f,8.5f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(10.5f,100.5f),Math::Vec2(9.5f,12.5f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(9.5f,8.5f),Math::Vec2(9.5f,12.5f),Math::Vec2(9.5f,10.5f),Math::Vec2(9.5f,10.5f));
}

TEST(CalcTestClosestPointToLine, Diagonal)
{
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,100.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(50.0f,50.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,50.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(80.0f,40.0f));
}

TEST(CalcTestClosestPointToLine, Extreme)
{
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.000001f,0.0f),Math::Vec2(-100.0f,0.0f),Math::Vec2(0.0f,0.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.000001f,0.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(0.000001f,0.0f));
	TEST_CLOSESTPOINTTOLINE(Math::Vec2(0.0f,0.0f),Math::Vec2(0.0f,0.0f),Math::Vec2(100.0f,0.0f),Math::Vec2(0.0f,0.0f));
}

} // namespace
