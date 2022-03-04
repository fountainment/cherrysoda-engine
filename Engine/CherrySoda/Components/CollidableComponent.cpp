#include <CherrySoda/Components/CollidableComponent.h>

#include <CherrySoda/Colliders/Collider.h>
#include <CherrySoda/Colliders/ColliderList.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Color.h>

using cherrysoda::CollidableComponent;

using cherrysoda::Camera;
using cherrysoda::Collider;
using cherrysoda::ColliderList;
using cherrysoda::Color;
using cherrysoda::Entity;

void CollidableComponent::Added(Entity* entity)
{
	base::Added(entity);
	if (m_collider != nullptr) {
		m_collider->m_entity = entity;
		if (m_collider->TypeID() == ColliderList::ColliderTypeID()) {
			for (auto c : *static_cast<ColliderList*>(m_collider)) {
				c->m_entity = entity;
			}
		}
	}
}

void CollidableComponent::Removed(Entity* entity)
{
	if (AutoDeleteEnabled()) {
		SetCollider(nullptr);
	}
	if (m_collider != nullptr) {
		m_collider->m_entity = nullptr;
		if (m_collider->TypeID() == ColliderList::ColliderTypeID()) {
			for (auto c : *static_cast<ColliderList*>(m_collider)) {
				c->m_entity = nullptr;
			}
		}
	}
	base::Removed(entity);
}

void CollidableComponent::DebugRender(Camera* camera)
{
	if (m_collider != nullptr) {
		m_collider->Render(camera, Collidable() ? Color::White : Color::Gray);
	}
}

void CollidableComponent::SetCollider(Collider* collider)
{
	if (collider == m_collider) {
		return;
	}

	if (m_collider != nullptr) {
		m_collider->Removed();
	}
	m_collider = collider;
	if (m_collider != nullptr) {
		m_collider->Added(this);
	}
}

Collider* CollidableComponent::GetCollider() const
{
	if (m_collider == nullptr) {
		return GetEntity()->GetCollider();
	}
	else {
		return m_collider;
	}
}
