#ifndef _CHERRYSODA_UTIL_COLOR_H_
#define _CHERRYSODA_UTIL_COLOR_H_

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Color
{
public:
	constexpr Color() : Color(0.0f) {}

	constexpr Color(float rgb, float a) : Color(rgb, rgb, rgb, a) {}
	constexpr Color(float r, float g, float b) : Color(r, g, b, 1.0f) {}
	constexpr Color(float r, float g, float b, float a)
		: m_r(r), m_g(g), m_b(b), m_a(a) {}

	explicit constexpr Color(float rgb) : Color(rgb, rgb, rgb) {}
	explicit Color(const Math::Vec3& v3) : Color(v3[0], v3[1], v3[2]) {}
	explicit Color(const Math::Vec4& v4) : Color(v4[0], v4[1], v4[2], v4[3]) {}

	constexpr Color(int r, int g, int b) : Color(r, g, b, 255) {}
	constexpr Color(int r, int g, int b, int a)
		: Color(r/255.f, g/255.f, b/255.f, a/255.f) {}

	constexpr Color(const char* str)
		: Color(0.0f)
	{
		int len = 0;
		while (str[len] != '\0') len++;
		CHERRYSODA_ASSERT(len > 3 && str[0] == '#', "Color string invalid!\n");
		type::UInt32 r = 255;
		type::UInt32 g = 255;
		type::UInt32 b = 255;
		type::UInt32 a = 255;
		switch (len) {
		case 4:
			r = StringUtil::HexStrToUInt32(str + 1, 1); r = (r << 4) | r;
			g = StringUtil::HexStrToUInt32(str + 2, 1); g = (g << 4) | g;
			b = StringUtil::HexStrToUInt32(str + 3, 1); b = (b << 4) | b;
			break;
		case 5:
			r = StringUtil::HexStrToUInt32(str + 1, 1); r = (r << 4) | r;
			g = StringUtil::HexStrToUInt32(str + 2, 1); g = (g << 4) | g;
			b = StringUtil::HexStrToUInt32(str + 3, 1); b = (b << 4) | b;
			a = StringUtil::HexStrToUInt32(str + 4, 1); a = (a << 4) | a;
			break;
		case 7:
			r = StringUtil::HexStrToUInt32(str + 1, 2);
			g = StringUtil::HexStrToUInt32(str + 3, 2);
			b = StringUtil::HexStrToUInt32(str + 5, 2);
			break;
		case 9:
			r = StringUtil::HexStrToUInt32(str + 1, 2);
			g = StringUtil::HexStrToUInt32(str + 3, 2);
			b = StringUtil::HexStrToUInt32(str + 5, 2);
			a = StringUtil::HexStrToUInt32(str + 7, 2);
			break;
		default:
			CHERRYSODA_ASSERT(false, "Color string length incorrect!\n");
			break;
		}
		m_r = r / 255.f;
		m_g = g / 255.f;
		m_b = b / 255.f;
		m_a = a / 255.f;
	}

	constexpr float R() const { return m_r; }
	constexpr float G() const { return m_g; }
	constexpr float B() const { return m_b; }
	constexpr float A() const { return m_a; }

	static constexpr type::UInt8  F2U8(float f)  { return static_cast<type::UInt8 >(f * 255.0f + 0.5f); }
	static constexpr type::UInt32 F2U32(float f) { return static_cast<type::UInt32>(f * 255.0f + 0.5f); }

	constexpr type::UInt32 U32()     const { return F2U32(R()) << 24 | F2U32(G()) << 16 | F2U32(B()) << 8 | F2U32(A()); }
	constexpr type::UInt32 U32ABGR() const { return F2U32(A()) << 24 | F2U32(B()) << 16 | F2U32(G()) << 8 | F2U32(R()); }

	inline void R(float v) { m_r = v; }
	inline void G(float v) { m_g = v; }
	inline void B(float v) { m_b = v; }
	inline void A(float v) { m_a = v; }
	inline void R(int v) { R(v / 255.f); }
	inline void G(int v) { G(v / 255.f); }
	inline void B(int v) { B(v / 255.f); }
	inline void A(int v) { A(v / 255.f); }

	constexpr Color operator + (const Color& other) const { return Color(m_r + other.m_r, m_g + other.m_g, m_b + other.m_b, m_a + other.m_a); }
	constexpr Color operator - (const Color& other) const { return Color(m_r - other.m_r, m_g - other.m_g, m_b - other.m_b, m_a - other.m_a); }
	constexpr Color operator * (float scale) const { return Color(m_r * scale, m_g * scale, m_b * scale, m_a * scale); }
	constexpr void operator *= (float scale) { *this = *this * scale; }

	constexpr bool operator == (const Color& other) const { return (m_r == other.m_r) && (m_g == other.m_g) && (m_b == other.m_b) && (m_a == other.m_a); }
	constexpr bool operator != (const Color& other) const { return (m_r != other.m_r) || (m_g != other.m_g) || (m_b != other.m_b) || (m_a != other.m_a); }

	static constexpr Color Lerp(const Color& color1, const Color& color2, float ease)
	{
		return color1 + (color2 - color1) * ease;
	}

	static const Color Transparent;
	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color DarkRed;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
	static const Color DarkYellow;
	static const Color Orange;
	static const Color Gray;

private:
	float m_r;
	float m_g;
	float m_b;
	float m_a;
};

} // namespace cherrysoda

#endif // _CHERRRYSODA_UTIL_COLOR_H_
