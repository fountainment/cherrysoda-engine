#include <CherrySoda/Entity.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/InternalUtilities/ComponentList.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Entity;

using cherrysoda::Camera;
using cherrysoda::Component;
using cherrysoda::ComponentList;
using cherrysoda::Math;
using cherrysoda::Scene;

Entity::Entity(const Math::Vec3& position)
{
	m_position = position;
	m_components = new ComponentList(this);
}

void Entity::SceneBegin(Scene* scene)
{
}

void Entity::SceneEnd(Scene* scene)
{
	for (auto component : *m_components) {
		component->SceneEnd(scene);
	}
}

void Entity::Awake(Scene* scene)
{
	for (auto component : *m_components) {
		component->EntityAwake();
	}
}

void Entity::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_components->Update();
}

void Entity::Render()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_components->Render();
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

void Entity::Depth(int depth)
{
	if (m_depth != depth) {
		m_depth = depth;
		if (m_scene != nullptr) {
			m_scene->_SetActualDepth(this);
		}
	}
}

void Entity::DebugRender(Camera* camera)
{
	// TODO: collider debugrender
	m_components->DebugRender(camera);
}

void Entity::HandleGraphicsReset()
{
	m_components->HandleGraphicsReset();
}

void Entity::HandleGraphicsCreate()
{
	m_components->HandleGraphicsCreate();
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
	m_scene->_SetActualDepth(this);
}

void Entity::Removed(Scene* scene)
{
	for (auto component : *m_components) {
		component->EntityRemoved(scene);
	}
	m_scene = nullptr;
}
