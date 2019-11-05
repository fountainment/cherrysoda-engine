#include <CherrySoda/Scene.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/InternalUtilities/RendererList.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Scene;

using cherrysoda::Engine;
using cherrysoda::EntityList;
using cherrysoda::RendererList;
using cherrysoda::STL;

Scene::Scene()
{
	m_entities = new EntityList(this);
	m_rendererList = new RendererList(this);
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
	if (!m_paused) {
		m_timeActive += Engine::GetInstance()->DeltaTime();
	}
	m_rawTimeActive += Engine::GetInstance()->RawDeltaTime();

	m_entities->UpdateLists();
	// TODO: m_tagLists->UpdateLists();
	m_rendererList->UpdateLists();
}

void Scene::Update()
{
	if (!m_paused) {
		m_entities->Update();
		m_rendererList->Update();
	}
}

void Scene::AfterUpdate()
{
	if (STL::Count(m_onEndOfFrame) > 0) {
		for (auto func : m_onEndOfFrame) {
			func();
		}
		m_onEndOfFrame.clear();
	}
}

void Scene::BeforeRender()
{
	m_rendererList->BeforeRender();
}

void Scene::Render()
{
	m_rendererList->Render();
}

void Scene::AfterRender()
{
	m_rendererList->AfterRender();
}

void Scene::OnEndOfFrame(STL::Action func)
{
	STL::Add(m_onEndOfFrame, func);
}

void Scene::HandleGraphicsCreate()
{
	m_entities->HandleGraphicsCreate();
}

void Scene::HandleGraphicsReset()
{
	m_entities->HandleGraphicsReset();
}