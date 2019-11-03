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
		// TODO: m_renderList->Update();
	}
}

void Scene::AfterUpdate()
{
	if (m_onEndOfFrame.size() > 0) {
		for (auto func : m_onEndOfFrame) {
			func();
		}
		m_onEndOfFrame.clear();
	}
}

void Scene::BeforeRender()
{
	// TODO: m_renderList->BeforeRender();
}

void Scene::Render()
{
	// TODO: m_renderList->Render();
}

void Scene::AfterRender()
{
	// TODO: m_renderList->AfterRender();
}

void Scene::OnEndOfFrame(std::function<void()> func)
{
	m_onEndOfFrame.emplace_back(func);
}