#include "MainScene.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Math.h>

#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Math;
using cherrysoda::Renderer;

void MainScene::Begin()
{
	base::Begin();

	m_renderer = new EverythingRenderer;
	m_chunk = new Chunk;

	m_renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 30.f));

	Add(m_renderer);
	Add(m_chunk);
}

void MainScene::BeforeRender()
{
	m_renderer->GetCamera()->Width(Engine::Instance()->GetWidth());
	m_renderer->GetCamera()->Height(Engine::Instance()->GetHeight());

	base::BeforeRender();
}