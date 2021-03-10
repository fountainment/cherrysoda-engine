#include <CherrySoda/Colliders/Hitbox.h>

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/Circle.h>

using cherrysoda::Hitbox;

using cherrysoda::Collide;

bool Hitbox::Collide(const Circle* circle) const
{
	// TODO
	// return Collide::RectToCircle(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, circle->AbsolutePosition2D(), circle->Radius());
	return false;
}

bool Hitbox::Collide(const Hitbox* hitbox) const
{
	return Intersects(hitbox);
}

bool Hitbox::Collide(const Math::Vec2& point) const
{
	return Collide::RectToPoint(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, point);
}

void Hitbox::Render(const Camera* camera, const Color& color) const
{
	// TODO
}
