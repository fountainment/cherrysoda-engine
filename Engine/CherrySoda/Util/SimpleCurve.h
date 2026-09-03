#ifndef _CHERRYSODA_UTIL_SIMPLECURVE_H_
#define _CHERRYSODA_UTIL_SIMPLECURVE_H_

#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

// Quadratic bezier curve between Begin and End bending through Control.
// Port of Monocle's SimpleCurve.
struct SimpleCurve
{
	SimpleCurve() = default;
	SimpleCurve(const Math::Vec2& begin, const Math::Vec2& end, const Math::Vec2& control)
		: m_begin(begin), m_end(end), m_control(control)
	{
	}

	void DoubleControl()
	{
		Math::Vec2 axis = m_end - m_begin;
		Math::Vec2 mid = m_begin + axis / 2.f;
		Math::Vec2 diff = m_control - mid;
		m_control += diff;
	}

	Math::Vec2 GetPoint(float percent) const
	{
		float reverse = 1.f - percent;
		return (reverse * reverse * m_begin) + (2.f * reverse * percent * m_control) + (percent * percent * m_end);
	}

	// Sums segment lengths over the given number of samples
	float GetLengthParametric(int resolution) const
	{
		Math::Vec2 last = m_begin;
		float length = 0.f;
		for (int i = 1; i <= resolution; i++) {
			Math::Vec2 at = GetPoint(i / static_cast<float>(resolution));
			length += Math_Length(at - last);
			last = at;
		}
		return length;
	}

	void Render(const Color& color, int resolution) const { Render(Vec2_Zero, color, resolution); }
	void Render(const Color& color, int resolution, float thickness) const
	{
		Render(Vec2_Zero, color, resolution, thickness);
	}
	void Render(const Math::Vec2& offset, const Color& color, int resolution) const;
	void Render(const Math::Vec2& offset, const Color& color, int resolution, float thickness) const;

	Math::Vec2 m_begin = Vec2_Zero;
	Math::Vec2 m_end = Vec2_Zero;
	Math::Vec2 m_control = Vec2_Zero;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_SIMPLECURVE_H_
