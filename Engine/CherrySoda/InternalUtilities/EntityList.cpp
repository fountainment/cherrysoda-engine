#include <CherrySoda/InternalUtilities/EntityList.h>

#include <CherrySoda/Entity.h>

#include <algorithm>

using cherrysoda::EntityList;

using cherrysoda::Entity;
using cherrysoda::Scene;

EntityList::EntityList(Scene* scene)
{
	m_scene = scene;
}

void EntityList::UpdateLists()
{
	// TODO: implement UpdateLists
	if (m_toAdd.size() > 0) {

	}

	if (m_toRemove.size() > 0) {

	}

	if (m_toAdd.size() > 0) {

	}
}

void EntityList::Add(Entity* entity)
{
	m_entities.push_back(entity);
}

void EntityList::Remove(Entity* entity)
{
	auto it = std::find(m_entities.begin(), m_entities.end(), entity);
	//CHERRYSODA_ASSERT(it != m_entities.end());
	m_entities.erase(it);
}

void EntityList::Update()
{
	for (auto entity : m_entities) {
		if (entity->m_active) entity->Update();
	}
}

void EntityList::Render()
{
	for (auto entity : m_entities) {
		if (entity->m_visible) entity->Render();
	}
}