#ifndef _CHERRYSODA_TEST_H_
#define _CHERRYSODA_TEST_H_

#include <CherrySoda/CherrySoda.h>
#include <gtest/gtest.h>

using namespace cherrysoda;

class Vec2TestWrapper
{
public:
	Vec2TestWrapper(const Math::Vec2& v2)
	: x(v2.x)
	, y(v2.y)
	{}

	friend void PrintTo(const Vec2TestWrapper& v2, std::ostream* os) {
		*os << "Math::Vec2(" << v2.x << "," << v2.y << ")";
	}

	friend bool operator==(const Vec2TestWrapper& va, const Vec2TestWrapper& vb)
	{
		return va.x == vb.x && va.y == vb.y;
	}

private:
	float x;
	float y;
};

#define TEST_CLOSESTPOINTTOLINE(a,b,p,e) \
{ \
	auto expectedResult = Vec2TestWrapper(e); \
	EXPECT_EQ(Vec2TestWrapper(Calc::ClosestPointToLine(a,b,p)),expectedResult); \
}

#endif // _CHERRYSODA_TEST_H_
