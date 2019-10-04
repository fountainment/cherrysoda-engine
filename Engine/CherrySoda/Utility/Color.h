#ifndef _CHERRYSODA_COLOR_H_
#define _CHERRYSODA_COLOR_H_

namespace cherrysoda {

class Color
{
public:
	constexpr Color() : Color(0.0f) {}
	constexpr Color(float rgb) : Color(rgb, rgb, rgb) {}
	constexpr Color(float r, float g, float b) : Color(r, g, b, 1.0f) {}
	constexpr Color(float r, float g, float b, float a)
		: m_r(r), m_g(g), m_b(b), m_a(a) {}

	constexpr float r() const { return m_r; }
	constexpr float g() const { return m_g; }
	constexpr float b() const { return m_b; }
	constexpr float a() const { return m_a; }

private:
	float m_r;
	float m_g;
	float m_b;
	float m_a;
};

class ColorValue {
public:
	static constexpr Color Black = Color(0.0f);
	static constexpr Color White = Color(1.0f);
};

} // namespace cherrysoda

#endif // _CHERRRYSODA_COLOR_H_