#include <CherrySoda/Colliders/Circle.h>

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Circle;

using cherrysoda::Camera;
using cherrysoda::Collide;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Hitbox;
using cherrysoda::Math;

bool Circle::Collide(const Circle* circle) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - circle->AbsolutePosition2D();
	return Math_LengthSq(vec) <= (Radius() + circle->Radius()) * (Radius() + circle->Radius());
}

bool Circle::Collide(const Hitbox* hitbox) const
{
	// TODO
	return false;
}

bool Circle::Collide(const Math::Vec2& point) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - point;
	return Math_LengthSq(vec) <= RadiusSq();
}

void Circle::Render(const Camera* camera, const Color& color) const
{
	Draw::Circle(AbsolutePosition2D(), Radius(), color, 4.f);
}
