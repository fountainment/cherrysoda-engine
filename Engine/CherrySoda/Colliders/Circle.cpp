#include <CherrySoda/Colliders/Circle.h>

#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Circle;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Math;

bool Circle::Collide(const Circle* circle) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - circle->AbsolutePosition2D();
	return Math_Dot(vec, vec) < (Radius() + circle->Radius()) * (Radius() + circle->Radius());
}

void Circle::Render(const Camera* camera, const Color& color) const
{
	// TODO:
}
