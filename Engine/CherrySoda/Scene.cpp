#include <CherrySoda/Scene.h>

using cherrysoda::Scene;

using cherrysoda::EntityList;

Scene::Scene()
{
	m_entities = new EntityList(this);
	// TODO: finish Scene::Scene
}

void Scene::Begin()
{
	m_focused = true;
	for (auto entity : *m_entities) {
		entity->SceneBegin(this);
	}
}

void Scene::End()
{
	m_focused = false;
	for (auto entity : *m_entities) {
		entity->SceneEnd(this);
	}
}

void Scene::BeforeUpdate()
{
	m_entities->UpdateLists();
}

void Scene::Update()
{
	if (!m_paused) {
		m_entities->Update();
	}
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