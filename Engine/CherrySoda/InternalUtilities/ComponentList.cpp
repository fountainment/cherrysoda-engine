#include <CherrySoda/InternalUtilities/ComponentList.h>

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::ComponentList;

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::Entity;
using cherrysoda::STL;

ComponentList::ComponentList(Entity* entity)
{
	m_entity = entity;
}

void ComponentList::Add(Component* component)
{
	STL::Add(m_components, component);
	component->Added(m_entity);
}

void ComponentList::Remove(Component* component)
{
	STL::Remove(m_components, component);
	component->Removed(m_entity);
}

void ComponentList::Add(ComponentList::IterableComponents& components)
{
	for (auto component : components) {
		Add(component);
	}
}

void ComponentList::Remove(ComponentList::IterableComponents& components)
{
	for (auto component : components) {
		Remove(component);
	}
}

void ComponentList::Update()
{
	for (auto component : m_components) {
		if (component->m_active) {
			component->Update();
		}
	}
}

void ComponentList::Render()
{
	for (auto component : m_components) {
		if (component->m_visible) {
			component->Render();
		}
	}
}

void ComponentList::DebugRender(const Camera* camera)
{
}