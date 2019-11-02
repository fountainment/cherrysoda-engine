#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <CherrySoda/Components/Component.h>

#include <algorithm>

using cherrysoda::ComponentList;

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::Entity;

ComponentList::ComponentList(Entity* entity)
{
	m_entity = entity;
}

void ComponentList::Add(Component* component)
{
	m_components.push_back(component);
	component->Added(m_entity);
}

void ComponentList::Remove(Component* component)
{
	auto it = std::find(m_components.begin(), m_components.end(), component);
	//CHERRYSODA_ASSERT(it != m_components.end());
	m_components.erase(it);
	component->Removed(m_entity);
}

void ComponentList::Update()
{
	for (auto comp : m_components) {
		if (comp->m_active) comp->Update();
	}
}

void ComponentList::Render()
{
	for (auto comp : m_components) {
		if (comp->m_visible) comp->Render();
	}
}

void ComponentList::DebugRender(const Camera* camera)
{
}