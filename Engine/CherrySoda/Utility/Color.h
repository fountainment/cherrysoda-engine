#ifndef _CHERRYSODA_COLOR_H_
#define _CHERRYSODA_COLOR_H_

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

	constexpr float R() const { return m_r; }
	constexpr float G() const { return m_g; }
	constexpr float B() const { return m_b; }
	constexpr float A() const { return m_a; }

	inline void SetR(float v) { m_r = v; }
	inline void SetG(float v) { m_g = v; }
	inline void SetB(float v) { m_b = v; }
	inline void SetA(float v) { m_a = v; }

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

class ColorValue {
public:

};

} // namespace cherrysoda

#endif // _CHERRRYSODA_COLOR_H_