#include <CherrySoda/InternalUtilities/RendererList.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::RendererList;

using cherrysoda::Renderer;
using cherrysoda::Scene;
using cherrysoda::Draw;
using cherrysoda::STL;

RendererList::RendererList(Scene* scene)
{
	m_scene = scene;
}

void RendererList::MoveToFront(Renderer* renderer)
{
	STL::Remove(m_renderers, renderer);
	STL::Add(m_renderers, renderer);
}

void RendererList::Add(Renderer* renderer)
{
	STL::Add(m_adding, renderer);
}

void RendererList::Remove(Renderer* renderer)
{
	STL::Add(m_removing, renderer);
}

void RendererList::UpdateLists()
{
	if (STL::IsNotEmpty(m_adding))
		for (auto renderer : m_adding)
			STL::Add(m_renderers, renderer);
	m_adding.clear();
	if (STL::IsNotEmpty(m_removing))
		for (auto renderer : m_removing)
			STL::Remove(m_renderers, renderer);
	m_removing.clear();
}

void RendererList::Update()
{
	for (auto renderer : m_renderers) {
		renderer->Update(m_scene);
	}
}

void RendererList::BeforeRender()
{
	for (auto renderer : m_renderers) {
		if (!renderer->Visible()) continue;
		Draw::SetRenderer(renderer);
		renderer->BeforeRender(m_scene);
	}
}

void RendererList::Render()
{
	for (auto renderer : m_renderers) {
		if (!renderer->Visible()) continue;
		Draw::SetRenderer(renderer);
		renderer->RenderPrepare(m_scene);
		renderer->Render(m_scene);
	}
}

void RendererList::AfterRender()
{
	for (auto renderer : m_renderers) {
		if (!renderer->Visible()) continue;
		Draw::SetRenderer(renderer);
		renderer->AfterRender(m_scene);
	}
}

Renderer* RendererList::First()
{
	Renderer* first = nullptr;
	STL::TryGetFirst(m_renderers, first);
	return first;
}
