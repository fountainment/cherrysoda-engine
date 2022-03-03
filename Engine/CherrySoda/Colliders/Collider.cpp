#include <CherrySoda/Colliders/Collider.h>

#include <CherrySoda/Colliders/Circle.h>
#include <CherrySoda/Colliders/ColliderList.h>
#include <CherrySoda/Colliders/Grid.h>
#include <CherrySoda/Colliders/Hitbox.h>
#include <CherrySoda/Components/CollidableComponent.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Pool.h>

using cherrysoda::Collider;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::CollidableComponent;
using cherrysoda::ColliderList;
using cherrysoda::Component;
using cherrysoda::Circle;
using cherrysoda::Entity;
using cherrysoda::Grid;
using cherrysoda::Hitbox;
using cherrysoda::Math;
using cherrysoda::PoolInterface;

bool Collider::Collide(const CollidableComponent* component) const
{
	return Collide(component->GetCollider());
}

bool Collider::Collide(const Collider* collider) const
{
	const auto typeID = collider->TypeID();
	if (typeID == Hitbox::ColliderTypeID()) {
		return Collide(static_cast<const Hitbox*>(collider));
	}
	else if (typeID == Grid::ColliderTypeID()) {
		return Collide(static_cast<const Grid*>(collider));
	}
	else if (typeID == ColliderList::ColliderTypeID()) {
		return Collide(static_cast<const ColliderList*>(collider));
	}
	else if (typeID == Circle::ColliderTypeID()) {
		return Collide(static_cast<const Circle*>(collider));
	}
	CHERRYSODA_DEBUG("Collisions against the collider type are not implemented!\n");
	return false;
}

void Collider::Added(Entity* entity)
{
	m_entity = entity;
	m_component = nullptr;
}

void Collider::Added(Component* component)
{
	m_entity = component->GetEntity();
	m_component = component;
}

void Collider::Removed()
{
	if (m_onRemoved != nullptr) {
		m_onRemoved(this, m_entity);
	}
	m_entity = nullptr;
	m_component = nullptr;
}

void Collider::Render(const Camera* camera) const
{
	Render(camera, Color::Yellow);
}

void Collider::AutoDeleteWhenRemoved(PoolInterface* pool)
{
	m_onRemoved =
		[pool](Collider* collider, Entity* entity)
		{
			pool->INTERNAL_Hide(collider);
			entity->GetScene()->AddActionOnEndOfFrame(
				[collider, pool]()
				{
					pool->INTERNAL_Destroy(collider);
				});
		};
}

void Collider::DeleteCollider(Collider* collider, Entity* entity)
{
	entity->GetScene()->AddActionOnEndOfFrame([collider](){ delete collider; });
}
