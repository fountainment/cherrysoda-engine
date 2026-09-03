#include "Test.h"

namespace {

TEST(MathTestRectangle, GettersAndClamp)
{
	Math::Rectangle r{Math::Vec2(1.f, 2.f), Math::Vec2(3.f, 4.f)};

	EXPECT_FLOAT_EQ(1.f, r.Left());
	EXPECT_FLOAT_EQ(4.f, r.Right());
	EXPECT_FLOAT_EQ(2.f, r.Bottom());
	EXPECT_FLOAT_EQ(6.f, r.Top());
	EXPECT_FLOAT_EQ(3.f, r.Width());
	EXPECT_FLOAT_EQ(4.f, r.Height());
	EXPECT_EQ(Math::Vec2(3.f, 4.f), r.Size());

	// Clamp pulls the point inside the rect
	EXPECT_EQ(Math::Vec2(1.f, 2.f), r.Clamp(Math::Vec2(-10.f, -10.f)));
	EXPECT_EQ(Math::Vec2(4.f, 6.f), r.Clamp(Math::Vec2(10.f, 10.f)));
	EXPECT_EQ(Math::Vec2(2.f, 3.f), r.Clamp(Math::Vec2(2.f, 3.f)));
}

TEST(MathTestRectangle, Move)
{
	Math::Rectangle r{Math::Vec2(1.f, 2.f), Math::Vec2(3.f, 4.f)};
	r.Move(Math::Vec2(10.f, -1.f));
	EXPECT_EQ(Math::Vec2(11.f, 1.f), r.m_coord);
	EXPECT_FLOAT_EQ(11.f, r.Left());
	EXPECT_FLOAT_EQ(5.f, r.Top());
}

TEST(MathTestRectangle, Intersects)
{
	Math::Rectangle r{Math::Vec2(0.f, 0.f), Math::Vec2(10.f, 10.f)};

	EXPECT_TRUE(r.Intersects(Math::Rectangle{Math::Vec2(5.f, 5.f), Math::Vec2(10.f, 10.f)}));
	EXPECT_TRUE(r.Intersects(Math::Rectangle{Math::Vec2(-5.f, -5.f), Math::Vec2(10.f, 10.f)}));
	EXPECT_FALSE(r.Intersects(Math::Rectangle{Math::Vec2(10.f, 0.f), Math::Vec2(5.f, 5.f)})); // touching right edge
	EXPECT_FALSE(r.Intersects(Math::Rectangle{Math::Vec2(0.f, 10.f), Math::Vec2(5.f, 5.f)})); // touching top edge
	EXPECT_FALSE(r.Intersects(Math::Rectangle{Math::Vec2(100.f, 100.f), Math::Vec2(5.f, 5.f)}));
}

TEST(MathTestIRectangle, EqualityAndClamp)
{
	Math::IRectangle a{Math::IVec2(0, 0), Math::IVec2(10, 10)};
	Math::IRectangle b{Math::IVec2(0, 0), Math::IVec2(10, 10)};
	Math::IRectangle c{Math::IVec2(1, 0), Math::IVec2(10, 10)};

	EXPECT_TRUE(a == b);
	EXPECT_FALSE(a == c);
	EXPECT_TRUE(a != c);
	EXPECT_FALSE(a != b);

	EXPECT_EQ(Math::IVec2(10, 10), a.Clamp(Math::IVec2(100, 100)));
	EXPECT_EQ(Math::IVec2(0, 0), a.Clamp(Math::IVec2(-5, -5)));
	EXPECT_EQ(Math::IVec2(5, 5), a.Clamp(Math::IVec2(5, 5)));

	a.Move(Math::IVec2(1, 2));
	EXPECT_EQ(Math::IVec2(1, 2), a.m_coord);
	EXPECT_EQ(11, a.Right());
	EXPECT_EQ(12, a.Top());
}

TEST(MathTestBitCount, Values)
{
	EXPECT_EQ(0, Math::BitCount_(0u));
	EXPECT_EQ(1, Math::BitCount_(1u));
	EXPECT_EQ(3, Math::BitCount_(0xbu));
	EXPECT_EQ(8, Math::BitCount_(0xffu));
	EXPECT_EQ(16, Math::BitCount_(static_cast<type::UInt16>(0xFFFFu)));
	EXPECT_EQ(32, Math::BitCount_(0xFFFFFFFFu));
	EXPECT_EQ(64, Math::BitCount_(0xFFFFFFFFFFFFFFFFull));
}

TEST(MathTestRaycastAABB, HitInterval)
{
	Math::AABB box{Math::Vec3(5.f, -1.f, -1.f), Math::Vec3(6.f, 1.f, 1.f)};

	float t1 = -1.f;
	float t2 = -1.f;
	EXPECT_TRUE(Math::RaycastAABB_(Math::Vec3(0.f), Math::Vec3(1.f, 0.f, 0.f), box, &t1, &t2));
	EXPECT_FLOAT_EQ(5.f, t1);
	EXPECT_FLOAT_EQ(6.f, t2);

	// Ray pointing backwards hits the box behind the start
	EXPECT_TRUE(Math::RaycastAABB_(Math::Vec3(10.f, 0.f, 0.f), Math::Vec3(-1.f, 0.f, 0.f), box, &t1, &t2));
	EXPECT_FLOAT_EQ(4.f, t1);
	EXPECT_FLOAT_EQ(5.f, t2);
}

TEST(MathTestRaycastAABB, MissAndInside)
{
	Math::AABB box{Math::Vec3(5.f, -1.f, -1.f), Math::Vec3(6.f, 1.f, 1.f)};

	// Offset too far on Y, moving along X
	EXPECT_FALSE(Math::RaycastAABB_(Math::Vec3(0.f), Math::Vec3(1.f, 0.f, 0.f),
									Math::AABB{Math::Vec3(5.f, 2.f, -1.f), Math::Vec3(6.f, 3.f, 1.f)}));

	// Box behind the ray direction
	EXPECT_FALSE(Math::RaycastAABB_(Math::Vec3(0.f), Math::Vec3(-1.f, 0.f, 0.f), box));

	// Start inside the box: the interval begins at 0
	Math::AABB aroundOrigin{Math::Vec3(-1.f), Math::Vec3(1.f)};
	float t1 = -1.f;
	float t2 = -1.f;
	EXPECT_TRUE(Math::RaycastAABB_(Math::Vec3(0.f), Math::Vec3(1.f, 0.f, 0.f), aroundOrigin, &t1, &t2));
	EXPECT_FLOAT_EQ(0.f, t1);
	EXPECT_FLOAT_EQ(1.f, t2);
}

TEST(MathTestRotateVector, Vec2AndVec3)
{
	Math::Vec2 rotated = Math::RotateVector_(Math::Vec2(1.f, 0.f), Math::PiHalf);
	EXPECT_NEAR(0.f, rotated.x, 1e-5f);
	EXPECT_NEAR(1.f, rotated.y, 1e-5f);

	rotated = Math::RotateVector_(Math::Vec2(0.f, 1.f), -Math::PiHalf);
	EXPECT_NEAR(1.f, rotated.x, 1e-5f);
	EXPECT_NEAR(0.f, rotated.y, 1e-5f);

	// Full circle returns the input
	rotated = Math::RotateVector_(Math::Vec2(3.f, 4.f), Math::Pi2);
	EXPECT_NEAR(3.f, rotated.x, 1e-5f);
	EXPECT_NEAR(4.f, rotated.y, 1e-5f);

	// Rotation around the Y axis sends X up toward -Z
	Math::Vec3 rotated3 = Math::RotateVector_(Math::Vec3(1.f, 0.f, 0.f), Math::PiHalf, Vec3_YUp);
	EXPECT_NEAR(0.f, rotated3.x, 1e-5f);
	EXPECT_NEAR(0.f, rotated3.y, 1e-5f);
	EXPECT_NEAR(-1.f, rotated3.z, 1e-5f);
}

} // namespace
