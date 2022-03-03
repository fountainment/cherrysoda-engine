#include <CherrySoda/Components/Component.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Pool.h>

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::Entity;
using cherrysoda::PoolInterface;
using cherrysoda::Scene;

Component::Component(bool active, bool visible)
{
	m_active = active;
	m_visible = visible;
}

void Component::Added(Entity* entity)
{
	m_entity = entity;
	Scene* scene = entity->GetScene();
	if (scene != nullptr) {
		// TODO: scene->Tracker()->ComponentAdded(this);
	}
}

void Component::Removed(Entity* entity)
{
	Scene* scene = entity->GetScene();
	if (scene != nullptr) {
		// TODO: scene->Tracker()->ComponentRemoved(this);
	}
	if (m_onRemoved != nullptr) {
		m_onRemoved(this, entity);
	}
	m_entity = nullptr;
}

void Component::EntityAdded(Scene* scene)
{
	// TODO: scene->Tracker()->ComponentAdded(this);
}

void Component::EntityRemoved(Scene* scene)
{
	// TODO: scene->Tracker()->ComponentRemoved(this);
}

void Component::SceneEnd(Scene* scene)
{
}

void Component::EntityAwake()
{
}

void Component::Update()
{
}

void Component::Render()
{
}

void Component::DebugRender(Camera* camera)
{
}

void Component::HandleGraphicsReset()
{
}

void Component::HandleGraphicsCreate()
{
}

void Component::RemoveSelf()
{
	if (m_entity != nullptr) {
		m_entity->Remove(this);
	}
}

void Component::AutoDeleteWhenRemoved(PoolInterface* pool)
{
	m_onRemoved =
		[pool](Component* component, Entity* entity)
		{
			pool->INTERNAL_Hide(component);
			entity->GetScene()->AddActionOnEndOfFrame(
				[pool, component]()
				{
					pool->INTERNAL_Destroy(component);
				}
			);
		};
}

Scene* Component::GetScene() const
{
	return m_entity != nullptr ? m_entity->GetScene() : nullptr;
}

void Component::DeleteComponent(Component* component, Entity* entity)
{
	entity->GetScene()->AddActionOnEndOfFrame([component](){ delete component; });
}
