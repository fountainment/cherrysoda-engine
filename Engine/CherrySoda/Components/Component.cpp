#include <CherrySoda/Components/Component.h>

#include <CherrySoda/Entity.h>

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::Entity;
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

Scene* Component::GetScene() const
{
	return m_entity != nullptr ? m_entity->GetScene() : nullptr;
}
