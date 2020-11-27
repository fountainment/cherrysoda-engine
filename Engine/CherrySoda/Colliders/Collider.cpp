#include <CherrySoda/Colliders/Collider.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Collider;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Circle;
using cherrysoda::Entity;
using cherrysoda::Math;

bool Collider::Collide(const Collider* collider) const
{
	if (dynamic_cast<const Circle*>(collider) != nullptr) {
		return Collide(static_cast<const Circle*>(collider));
	}
	return false;
}

Math::Vec2 Collider::AbsolutePosition2D() const
{
	if (m_entity != nullptr) {
		return m_entity->Position2D() + Position2D();
	}
	return Position2D();
}

Math::Vec3 Collider::AbsolutePosition() const
{
	if (m_entity != nullptr) {
		return m_entity->Position() + Position();
	}
	return Position();
}

void Collider::Removed()
{
	m_entity = nullptr;
	m_component = nullptr;
}

void Collider::Added(Entity* entity)
{
	m_entity = entity;
	m_component = nullptr;
}

void Collider::Render(const Camera* camera) const
{
	Render(camera, Color::Red);
}
