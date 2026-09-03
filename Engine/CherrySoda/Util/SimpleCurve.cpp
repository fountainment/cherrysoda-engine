#include <CherrySoda/Util/SimpleCurve.h>

#include <CherrySoda/Util/Draw.h>

namespace cherrysoda {

void SimpleCurve::Render(const Math::Vec2& offset, const Color& color, int resolution) const
{
	Render(offset, color, resolution, 1.f);
}

void SimpleCurve::Render(const Math::Vec2& offset, const Color& color, int resolution, float thickness) const
{
	Math::Vec2 last = offset + m_begin;
	for (int i = 1; i <= resolution; i++) {
		Math::Vec2 at = offset + GetPoint(i / static_cast<float>(resolution));
		Draw::Line(last, at, color, thickness);
		last = at;
	}
}

} // namespace cherrysoda
