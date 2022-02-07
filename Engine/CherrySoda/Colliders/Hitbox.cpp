#include <CherrySoda/Colliders/Hitbox.h>

#include <CherrySoda/Colliders/Collide.h>
#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Hitbox;

using cherrysoda::Camera;
using cherrysoda::Circle;
using cherrysoda::Collide;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Math;

bool Hitbox::Collide(const Circle* circle) const
{
	return Collide::RectToCircle(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, circle->AbsolutePosition2D(), circle->Radius());
}

bool Hitbox::Collide(const Hitbox* hitbox) const
{
	return Intersects(hitbox);
}

bool Hitbox::Collide(const Math::Vec2& point) const
{
	return Collide::RectToPoint(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, point);
}

bool Hitbox::Collide(const Math::Vec2& from, const Math::Vec2& to) const
{
	return Collide::RectToLine(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, from, to);
}

void Hitbox::Render(const Camera* camera, const Color& color) const
{
	Draw::HollowRect(AbsoluteLeft(), AbsoluteBottom(), m_width, m_height, color);	
}
