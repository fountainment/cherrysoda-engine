#include <CherrySoda/Entity.h>

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <glm/vec2.hpp>

using cherrysoda::Entity;

using cherrysoda::Camera;
using cherrysoda::Component;

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

void Entity::DebugRender(const Camera* camera)
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

void Entity::RemoveSelf()
{
	if (m_scene != nullptr) {
		// m_scene->GetEntities()->Remove(this);
	}
}