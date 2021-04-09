#include <CherrySoda/Colliders/Collider.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/Hitbox.h>
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
	const auto typeID = collider->TypeID();
	if (typeID == Hitbox::ColliderTypeID()) {
		return Collide(static_cast<const Hitbox*>(collider));
	}
	else if (typeID == Circle::ColliderTypeID()) {
		return Collide(static_cast<const Circle*>(collider));
	}
	return false;
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
	Render(camera, Color::Yellow);
}
