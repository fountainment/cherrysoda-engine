#include <CherrySoda/Colliders/Circle.h>

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/ColliderList.h>
#include <CherrySoda/Colliders/Grid.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Circle;

using cherrysoda::Camera;
using cherrysoda::Collide;
using cherrysoda::ColliderList;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Grid;
using cherrysoda::Hitbox;
using cherrysoda::Math;

bool Circle::Collide(const Circle* circle) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - circle->AbsolutePosition2D();
	return Math_LengthSq(vec) <= (Radius() + circle->Radius()) * (Radius() + circle->Radius());
}

bool Circle::Collide(const ColliderList* list) const
{
	return list->Collide(this);
}

bool Circle::Collide(const Hitbox* hitbox) const
{
	return hitbox->Collide(this);
}

bool Circle::Collide(const Grid* grid) const
{
	return grid->Collide(this);
}

bool Circle::Collide(const Math::Vec2& point) const
{
	const Math::Vec2 vec = AbsolutePosition2D() - point;
	return Math_LengthSq(vec) <= RadiusSq();
}

bool Circle::Collide(const Math::Rectangle& rect) const
{
	return Collide::RectToCircle(rect, AbsolutePosition2D(), Radius());
}

bool Circle::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	return Collide::CircleToLine(AbsolutePosition2D(), Radius(), from, to);
}

void Circle::Render(const Camera* camera, const Color& color) const
{
	Draw::Circle(AbsolutePosition2D(), Radius(), color, 4.f);
}
