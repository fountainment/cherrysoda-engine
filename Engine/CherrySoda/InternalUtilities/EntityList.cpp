#include <CherrySoda/InternalUtilities/EntityList.h>

#include <CherrySoda/Entity.h>
#include <CherrySoda/InternalUtilities/TagLists.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::EntityList;

using cherrysoda::BitTagValueType;
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
	if (STL::IsNotEmpty(m_toAdd)) {
		for (auto entity : m_toAdd) {
			if (!STL::Contains(m_current, entity)) {
				STL::Add(m_current, entity);
				STL::Add(m_entities, entity);
			}

			if (m_scene != nullptr) {
				// TODO: Add Tracker
				m_scene->Tags()->EntityAdded(entity);
				// m_scene->Tracker()->EntityAdded(entity);
				entity->Added(m_scene);
			}
		}

		m_unsorted = true;
	}

	if (STL::IsNotEmpty(m_toRemove)) {
		for (auto entity : m_toRemove) {
			if (STL::Contains(m_entities, entity)) {
				STL::Remove(m_current, entity);
				STL::Remove(m_entities, entity);

				if (m_scene != nullptr) {
					entity->Removed(m_scene);
					// TODO: Add Tracker and Pooler
					m_scene->Tags()->EntityRemoved(entity);
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
		STL::Sort(m_entities, CompareDepth);
	}

	if (STL::IsNotEmpty(m_toAdd)) {
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
	if (!STL::Contains(m_removing, entity) && STL::Contains(m_current, entity)) {
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

void EntityList::RenderOnly(BitTagValueType matchTags)
{
	for (auto entity : m_entities) {
		if (entity->m_visible && entity->TagCheck(matchTags)) {
			entity->Render();
		}
	}
}

void EntityList::RenderOnlyFullMatch(BitTagValueType matchTags)
{
	for (auto entity : m_entities) {
		if (entity->m_visible && entity->TagFullCheck(matchTags)) {
			entity->Render();
		}
	}
}

void EntityList::RenderExcept(BitTagValueType excludeTags)
{
	for (auto entity : m_entities) {
		if (entity->m_visible && !entity->TagCheck(excludeTags)) {
			entity->Render();
		}
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

Entity* EntityList::First()
{
	Entity* first = nullptr;
	STL::TryGetFirst(m_entities, first);
	return first;
}
