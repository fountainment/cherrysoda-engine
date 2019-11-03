#include <CherrySoda/Entity.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <glm/vec2.hpp>

using cherrysoda::Entity;

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::ComponentList;
using cherrysoda::Scene;

Entity::Entity(const glm::vec2& position)
{
	m_position = position;
	m_components = new ComponentList(this);
}

void Entity::Update()
{
	m_components->Update();
}

void Entity::Render()
{
	m_components->Render();
}

void Entity::DebugRender(Camera* camera)
{
	m_components->DebugRender(camera);
}

void Entity::Add(Component* component)
{
	m_components->Add(component);
}

void Entity::Remove(Component* component)
{
	m_components->Remove(component);
}

void Entity::Add(ComponentList::IterableComponents& components)
{
	m_components->Add(components);
}

void Entity::Remove(ComponentList::IterableComponents& components)
{
	m_components->Remove(components);
}

void Entity::RemoveSelf()
{
	if (m_scene != nullptr) {
		m_scene->Entities()->Remove(this);
	}
}

void Entity::Added(Scene* scene)
{
	m_scene = scene;
	for (auto component : *m_components) {
		component->EntityAdded(scene);
	}
	// TODO: implement Scene::SetActualDepth	
	// m_scene->SetActualDepth(this);
}

void Entity::Removed(Scene* scene)
{
	for (auto component : *m_components) {
		component->EntityRemoved(scene);
	}
	m_scene = nullptr;
}