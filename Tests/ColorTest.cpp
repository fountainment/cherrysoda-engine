#include "Test.h"

namespace {

TEST(ColorTestConstruct, FloatAndIntOverloads)
{
	EXPECT_EQ(Color(1.0f, 0.0f, 0.0f), Color::Red);
	EXPECT_EQ(Color(1.0f, 0.0f, 0.0f, 1.0f), Color::Red);
	EXPECT_EQ(Color(255, 0, 0), Color::Red);
	EXPECT_EQ(Color(255, 0, 0, 255), Color::Red);
	EXPECT_EQ(Color(0.5f), Color::Gray);
	EXPECT_EQ(Color(0.5f, 0.5f, 0.0f), Color::DarkYellow);
	EXPECT_EQ(Color(0.5f, 0.0f, 0.0f), Color::DarkRed);

	// Default is opaque black; the (rgb, a) overload sets alpha
	EXPECT_FLOAT_EQ(1.0f, Color().A());
	EXPECT_FLOAT_EQ(0.0f, Color().R());
	EXPECT_FLOAT_EQ(0.0f, Color().G());
	EXPECT_FLOAT_EQ(0.0f, Color().B());
	EXPECT_FLOAT_EQ(0.0f, Color::Transparent.A());

	// Vec3/Vec4 explicit constructors
	EXPECT_EQ(Color(Math::Vec3(1.0f, 0.0f, 0.0f)), Color::Red);
	EXPECT_EQ(Color(Math::Vec4(1.0f, 0.0f, 0.0f, 1.0f)), Color::Red);
	EXPECT_FLOAT_EQ(0.5f, Color(Math::Vec4(1.0f, 0.0f, 0.0f, 0.5f)).A());

	// Setters
	Color c;
	c.R(1.0f);
	c.G(0.5f);
	c.B(0.25f);
	c.A(0.75f);
	EXPECT_FLOAT_EQ(1.0f, c.R());
	EXPECT_FLOAT_EQ(0.5f, c.G());
	EXPECT_FLOAT_EQ(0.25f, c.B());
	EXPECT_FLOAT_EQ(0.75f, c.A());

	Color cInt;
	cInt.R(255);
	cInt.G(128);
	cInt.B(0);
	cInt.A(64);
	EXPECT_FLOAT_EQ(1.0f, cInt.R());
	EXPECT_NEAR(128 / 255.f, cInt.G(), 1e-6f);
	EXPECT_FLOAT_EQ(0.0f, cInt.B());
	EXPECT_NEAR(64 / 255.f, cInt.A(), 1e-6f);
}

TEST(ColorTestHex, ParseAllLengths)
{
	// #RGB
	Color rgb("#F80");
	EXPECT_FLOAT_EQ(1.0f, rgb.R());
	EXPECT_NEAR(0x88 / 255.f, rgb.G(), 1e-6f);
	EXPECT_FLOAT_EQ(0.0f, rgb.B());
	EXPECT_FLOAT_EQ(1.0f, rgb.A());

	// #RGBA
	Color rgba("#F808");
	EXPECT_FLOAT_EQ(1.0f, rgba.R());
	EXPECT_NEAR(0x88 / 255.f, rgba.G(), 1e-6f);
	EXPECT_FLOAT_EQ(0.0f, rgba.B());
	EXPECT_NEAR(0x88 / 255.f, rgba.A(), 1e-6f);

	// #RRGGBB
	Color rrggbb("#FF8000");
	EXPECT_FLOAT_EQ(1.0f, rrggbb.R());
	EXPECT_NEAR(0x80 / 255.f, rrggbb.G(), 1e-6f);
	EXPECT_FLOAT_EQ(0.0f, rrggbb.B());
	EXPECT_FLOAT_EQ(1.0f, rrggbb.A());

	// #RRGGBBAA
	Color rrggbbaa("#FF800080");
	EXPECT_FLOAT_EQ(1.0f, rrggbbaa.R());
	EXPECT_NEAR(0x80 / 255.f, rrggbbaa.G(), 1e-6f);
	EXPECT_FLOAT_EQ(0.0f, rrggbbaa.B());
	EXPECT_NEAR(0x80 / 255.f, rrggbbaa.A(), 1e-6f);

	// Lowercase hex digits
	EXPECT_EQ(Color("#ff8000"), Color("#FF8000"));
	EXPECT_EQ(Color("#f80"), Color("#F80"));
}

TEST(ColorTestPacked, F2UAndU32)
{
	EXPECT_EQ(0, Color::F2U8(0.0f));
	EXPECT_EQ(128, Color::F2U8(0.5f));
	EXPECT_EQ(255, Color::F2U8(1.0f));
	EXPECT_EQ(255u, Color::F2U32(1.0f));

	// RGBA packing
	EXPECT_EQ(0xFFFFFFFFu, Color::White.U32());
	EXPECT_EQ(0xFF0000FFu, Color::Red.U32());
	EXPECT_EQ(0xFF80FFFFu, Color(1.0f, 0.5f, 1.0f, 1.0f).U32());

	// ABGR packing
	EXPECT_EQ(0xFFFFFFFFu, Color::White.U32ABGR());
	EXPECT_EQ(0xFF0000FFu, Color::Red.U32ABGR());
	EXPECT_EQ(0xFFFF80FFu, Color(1.0f, 0.5f, 1.0f, 1.0f).U32ABGR());
	EXPECT_EQ(0x80FFFFFFu, Color(1.0f, 1.0f, 1.0f, 0.5f).U32ABGR());
}

TEST(ColorTestLerp, EndpointsAndMid)
{
	EXPECT_EQ(Color::Black, Color::Lerp(Color::Black, Color::White, 0.f));
	EXPECT_EQ(Color::White, Color::Lerp(Color::Black, Color::White, 1.f));
	EXPECT_EQ(Color::Gray, Color::Lerp(Color::Black, Color::White, 0.5f));
	EXPECT_EQ(Color(0.5f, 0.0f, 0.5f), Color::Lerp(Color::Red, Color::Blue, 0.5f));
}

TEST(ColorTestOperators, ArithmeticAndEquality)
{
	// + and - operate on all four channels and do not clamp
	Color yellowOvershoot = Color::Red + Color::Green;
	EXPECT_FLOAT_EQ(1.f, yellowOvershoot.R());
	EXPECT_FLOAT_EQ(1.f, yellowOvershoot.G());
	EXPECT_FLOAT_EQ(0.f, yellowOvershoot.B());
	EXPECT_FLOAT_EQ(2.f, yellowOvershoot.A()); // 1 + 1

	Color whiteNoAlpha = Color::White - Color::Black;
	EXPECT_FLOAT_EQ(1.f, whiteNoAlpha.R());
	EXPECT_FLOAT_EQ(1.f, whiteNoAlpha.G());
	EXPECT_FLOAT_EQ(1.f, whiteNoAlpha.B());
	EXPECT_FLOAT_EQ(0.f, whiteNoAlpha.A()); // 1 - 1

	// operator* scales all four channels, alpha included
	Color halfRed = Color::Red * 0.5f;
	EXPECT_FLOAT_EQ(0.5f, halfRed.R());
	EXPECT_FLOAT_EQ(0.0f, halfRed.G());
	EXPECT_FLOAT_EQ(0.5f, halfRed.A());

	Color c = Color::White;
	c *= 0.5f;
	EXPECT_FLOAT_EQ(0.5f, c.R());
	EXPECT_FLOAT_EQ(0.5f, c.G());
	EXPECT_FLOAT_EQ(0.5f, c.B());
	EXPECT_FLOAT_EQ(0.5f, c.A());

	EXPECT_TRUE(Color::Red != Color::Green);
	EXPECT_FALSE(Color::Red != Color(255, 0, 0));
}

TEST(ColorTestPalette, Constants)
{
	EXPECT_EQ(Color(1.0f, 1.0f, 1.0f, 1.0f), Color::White);
	EXPECT_EQ(Color(1.0f, 1.0f, 0.0f, 1.0f), Color::Yellow);
	EXPECT_EQ(Color(1.0f, 0.5f, 0.0f, 1.0f), Color::Orange);
	EXPECT_EQ(Color(0.5f, 0.5f, 0.5f, 1.0f), Color::Gray);
	EXPECT_EQ(Color(0.0f, 0.0f, 1.0f, 1.0f), Color::Blue);
	EXPECT_EQ(Color(0.0f, 1.0f, 0.0f, 1.0f), Color::Green);
	EXPECT_EQ(Color(0.0f, 0.0f, 0.0f, 0.0f), Color::Transparent);
}

} // namespace
