#include <CherrySoda/InternalUtilities/EntityList.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::EntityList;

using cherrysoda::Camera;
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
		for (auto entity : m_toRemove) {
			if (STL::Contains(m_entities, entity)) {
				STL::Remove(m_current, entity);
				STL::Remove(m_entities, entity);

				if (m_scene != nullptr) {
					entity->Removed(m_scene);
					// TODO: m_scene->TagLists()->EntityRemoved(entity);
					// m_scene->Tracker()->EntityRemoved(entity);
					// Engine.Pooler.EntityRemoved(entity);
				}
			}
		}

		m_toRemove.clear();
		m_removing.clear();
	}

	if (m_unsorted) {
		m_unsorted = false;
		// TODO: STL::Sort(m_entities);	
	}

	if (STL::Count(m_toAdd) > 0) {
        STL::AddRange(m_toAwake, m_toAdd);
        m_toAdd.clear();
        m_adding.clear();

        for (auto entity : m_toAwake) {
            if (entity->GetScene() == m_scene) {
                entity->Awake(m_scene);
            }
        }
        m_toAwake.clear();
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

void EntityList::DebugRender(Camera* camera)
{
	for (auto entity : m_entities) {
		entity->DebugRender(camera);
	}
}

void EntityList::HandleGraphicsReset()
{
	for (auto entity : m_entities) {
		entity->HandleGraphicsReset();
	}
}

void EntityList::HandleGraphicsCreate()
{
	for (auto entity : m_entities) {
		entity->HandleGraphicsCreate();
	}
}