#include <CherrySoda/Scene.h>

using cherrysoda::Scene;

void Scene::Begin()
{
	m_focused = true;
	for (auto entity : m_entities) {
		entity->SceneBegin(this);
	}
}

void Scene::End()
{
	m_focused = false;
	for (auto entity : m_entities) {
		entity->SceneEnd(this);
	}
}

void Scene::BeforeUpdate()
{
	m_entities.UpdateLists();
}

void Scene::Update()
{
}

void Scene::AfterUpdate()
{
}

void Scene::Render()
{
}

void Scene::AfterRender()
{
}