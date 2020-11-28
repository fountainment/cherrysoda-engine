#ifndef _CHERRYSODA_UTIL_COLOR_H_
#define _CHERRYSODA_UTIL_COLOR_H_

#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class Color
{
public:
	constexpr Color() : Color(0.0f) {}
	constexpr Color(float rgb) : Color(rgb, rgb, rgb) {}
	constexpr Color(float rgb, float a) : Color(rgb, rgb, rgb, a) {}
	constexpr Color(float r, float g, float b) : Color(r, g, b, 1.0f) {}
	constexpr Color(float r, float g, float b, float a)
		: m_r(r), m_g(g), m_b(b), m_a(a) {}

	constexpr Color(int r, int g, int b) : Color(r, g, b, 255) {}
	constexpr Color(int r, int g, int b, int a)
		: Color(r/255.f, g/255.f, b/255.f, a/255.f) {}

	constexpr float R() const { return m_r; }
	constexpr float G() const { return m_g; }
	constexpr float B() const { return m_b; }
	constexpr float A() const { return m_a; }

	static constexpr type::UInt8  F2U8(float f)  { return static_cast<type::UInt8 >(f * 255.0f + 0.5f); }
	static constexpr type::UInt32 F2U32(float f) { return static_cast<type::UInt32>(f * 255.0f + 0.5f); }

	constexpr type::UInt32 U32() const { return U32BigEndian(); }
	constexpr type::UInt32 U32BigEndian()    const { return F2U32(R()) << 24 | F2U32(G()) << 16 | F2U32(B()) << 8  | F2U32(A()); }
	constexpr type::UInt32 U32LittleEndian() const { return F2U32(R())       | F2U32(G()) <<  8 | F2U32(B()) << 16 | F2U32(A()) << 24; }

	constexpr type::UInt32 U32ABGR() const { return U32ABGRBigEndian(); }
	constexpr type::UInt32 U32ABGRBigEndian()    const { return F2U32(A()) << 24 | F2U32(B()) << 16 | F2U32(G()) <<  8 | F2U32(R()); }
	constexpr type::UInt32 U32ABGRLittleEndian() const { return F2U32(A())       | F2U32(B()) <<  8 | F2U32(G()) << 16 | F2U32(R()) << 24; }

	inline void R(float v) { m_r = v; }
	inline void G(float v) { m_g = v; }
	inline void B(float v) { m_b = v; }
	inline void A(float v) { m_a = v; }
	inline void R(int v) { R(v / 255.f); }
	inline void G(int v) { G(v / 255.f); }
	inline void B(int v) { B(v / 255.f); }
	inline void A(int v) { A(v / 255.f); }

	constexpr Color operator * (float scale) const { return Color(m_r * scale, m_g * scale, m_b * scale, m_a * scale); }

	static const Color Black;
	static const Color White;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
	static const Color Yellow;
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
