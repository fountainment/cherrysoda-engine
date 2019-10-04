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

private:
	float m_r;
	float m_g;
	float m_b;
	float m_a;
};

class ColorValue {
public:
	static constexpr Color const& Black  = Color(0.0f);
	static constexpr Color const& White  = Color(1.0f);
	static constexpr Color const& Red    = Color(1.0f, 0.0f, 0.0f);
	static constexpr Color const& Green  = Color(0.0f, 1.0f, 0.0f);
	static constexpr Color const& Blue   = Color(0.0f, 0.0f, 1.0f);
	static constexpr Color const& Yellow = Color(1.0f, 1.0f, 0.0f);
	static constexpr Color const& Orange = Color(1.0f, 0.5f, 0.0f);
	static constexpr Color const& Gray   = Color(0.5f);
};

} // namespace cherrysoda

#endif // _CHERRRYSODA_COLOR_H_