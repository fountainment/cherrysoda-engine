#include <CherrySoda/Components/Component.h>

using cherrysoda::Component;
using cherrysoda::Entity;

Component::Component(bool active, bool visible)
{
	m_active = active;
	m_visible = visible;
}

void Component::Added(Entity* entity)
{
	m_entity = entity;
}

void Component::Removed(Entity* entity)
{
	m_entity = nullptr;
}