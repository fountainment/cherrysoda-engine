#include <CherrySoda/CherrySoda.h>
#include <gtest/gtest.h>

using namespace cherrysoda;

namespace {

TEST(ClosestPointToLine, Horizontal)
{
	{
		auto lineA = Math::Vec2(0.0f, 0.0f);
		auto lineB = Math::Vec2(5.0f, 0.0f);
		auto point = Math::Vec2(2.5f, 2.5f);
		auto intendedResult = Math::Vec2(2.5f, 0.0f);
		EXPECT_EQ(Calc::ClosestPointToLine(lineA, lineB, point), intendedResult);
	}
	{
		auto lineA = Math::Vec2(7.0f, 9.0f);
		auto lineB = Math::Vec2(15.0f, 9.0f);
		auto point = Math::Vec2(8.5f, 8.5f);
		auto intendedResult = Math::Vec2(8.5f, 9.0f);
		EXPECT_EQ(Calc::ClosestPointToLine(lineA, lineB, point), intendedResult);
	}
}

TEST(ClosestPointToLine, Vertical)
{
	{
		auto lineA = Math::Vec2(0.0f, 0.0f);
		auto lineB = Math::Vec2(0.0f, 5.0f);
		auto point = Math::Vec2(2.5f, 2.5f);
		auto intendedResult = Math::Vec2(0.0f, 2.5f);
		EXPECT_EQ(Calc::ClosestPointToLine(lineA, lineB, point), intendedResult);
	}
	{
		auto lineA = Math::Vec2(9.5f, 8.5f);
		auto lineB = Math::Vec2(9.5f, 12.5f);
		auto point = Math::Vec2(10.5f, 10.5f);
		auto intendedResult = Math::Vec2(9.5f, 10.5f);
		EXPECT_EQ(Calc::ClosestPointToLine(lineA, lineB, point), intendedResult);
	}
}

} // namespace
