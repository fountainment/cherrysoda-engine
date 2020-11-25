#include <CherrySoda/Scene.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/InternalUtilities/RendererList.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Scene;

using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::EntityList;
using cherrysoda::Renderer;
using cherrysoda::RendererList;
using cherrysoda::STL;

Scene::Scene()
{
	m_entities = new EntityList(this);
	m_rendererList = new RendererList(this);
	// TODO: finish Scene::Scene
}

Scene::~Scene()
{
	delete m_entities;
	delete m_rendererList;
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
	CHERRYSODA_PROFILE_FUNCTION();

	if (!m_paused) {
		m_timeActive += Engine::Instance()->DeltaTime();
	}
	m_rawTimeActive += Engine::Instance()->RawDeltaTime();

	m_entities->UpdateLists();
	// TODO: m_tagLists->UpdateLists();
	m_rendererList->UpdateLists();
}

void Scene::Update()
{
	CHERRYSODA_PROFILE_FUNCTION();

	if (!m_paused) {
		m_entities->Update();
		m_rendererList->Update();
	}
}

void Scene::AfterUpdate()
{
	CHERRYSODA_PROFILE_FUNCTION();

	if (!STL::Empty(m_onEndOfFrame)) {
		for (auto func : m_onEndOfFrame) {
			func();
		}
		m_onEndOfFrame.clear();
	}
}

void Scene::BeforeRender()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_rendererList->BeforeRender();
}

void Scene::Render()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_rendererList->Render();
}

void Scene::AfterRender()
{
	CHERRYSODA_PROFILE_FUNCTION();

	m_rendererList->AfterRender();
}

Renderer* Scene::FirstRenderer()
{
	return Renderers()->First();	
}

void Scene::AddActionOnEndOfFrame(STL::Action<> func)
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

void Scene::Add(Entity* entity)
{
	m_entities->Add(entity);
}

void Scene::Remove(Entity* entity)
{
	m_entities->Remove(entity);
}

void Scene::Add(Renderer* renderer)
{
	m_rendererList->Add(renderer);
}

void Scene::Remove(Renderer* renderer)
{
	m_rendererList->Remove(renderer);
}

void Scene::_SetActualDepth(Entity* entity)
{
	double value = 0.0;
	if (STL::TryGetValue(m_actualDepthLookup, entity->Depth(), value))
	{
		m_actualDepthLookup[entity->Depth()] += 9.9999999747524271E-07;
	}
	else
	{
		STL::Add(m_actualDepthLookup, STL::MakePair(entity->Depth(), 9.9999999747524271E-07));
	}
	entity->m_actualDepth = (double)entity->Depth() - value;
	m_entities->MarkUnsorted();	
	// TODO: Implement taglist
	// for (int i = 0; i < BitTag::TotalTags(); ++i)
	// {
	// 	if (entity->TagCheck(1 << i))
	// 	{
	// 		m_tagLists->MarkUnsorted(i);
	// 	}
	// }
}
