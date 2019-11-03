#include <CherrySoda/InternalUtilities/EntityList.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::EntityList;

using cherrysoda::Entity;
using cherrysoda::Scene;
using cherrysoda::STL;

EntityList::EntityList(Scene* scene)
{
	m_scene = scene;
}

void EntityList::UpdateLists()
{
	if (STL::Count(m_toAdd) > 0) {
		for (auto entity : m_toAdd) {
			if (!STL::Contains(m_current, entity)) {
				STL::Add(m_current, entity);
				STL::Add(m_entities, entity);
			}	

			if (m_scene != nullptr) {
				// TODO: Add Taglists and Tracker
				// m_scene->Taglists()->EntityAdded(entity);	
				// m_scene->Tracker()->EntityAdded(entity);	
				entity->Added(m_scene);	
			}
		}

		m_unsorted = true;
	}

	if (STL::Count(m_toRemove) > 0) {

	}

	if (STL::Count(m_toAdd) > 0) {

	}
}

void EntityList::Add(Entity* entity)
{
	if (!STL::Contains(m_adding, entity) && !STL::Contains(m_current, entity)) {
		STL::Add(m_adding, entity);
		STL::Add(m_toAdd, entity);
	}
}

void EntityList::Remove(Entity* entity)
{
	if (!STL::Contains(m_removing, entity) && !STL::Contains(m_current, entity)) {
		STL::Add(m_removing, entity);
		STL::Add(m_toRemove, entity);
	}
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