#include "MainScene.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Math.h>

#include "VoxelExperiment.h"
#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::Renderer;


class VoxelRenderer : public EverythingRenderer
{
public:
	void Render(cherrysoda::Scene* scene) override
	{
		Graphics::SetSamplerTexCube(&GameApp::ms_texCube);
		Graphics::SetSamplerTexCubeIrr(&GameApp::ms_texCubeIrr);

		EverythingRenderer::Render(scene);
	}
};

void MainScene::Begin()
{
	base::Begin();

	m_renderer = new VoxelRenderer;
	m_chunk = new Chunk;

	m_renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 30.f));
	
	Graphics::SetUniformCamPos(m_renderer->GetCamera()->Position());
	Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93, 0.88f), 1.f, 0.5f, 0.1f);
	Graphics::SetUniformLight(0, Math::Vec3(-5.f, 5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(1, Math::Vec3(5.f, 5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(2, Math::Vec3(-5.f, -5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(3, Math::Vec3(5.f, -5.f, 8.f), Math::Vec3(1.f));

	Add(m_renderer);
	Add(m_chunk);
}

void MainScene::BeforeRender()
{
	m_renderer->GetCamera()->Width(Engine::Instance()->GetWidth());
	m_renderer->GetCamera()->Height(Engine::Instance()->GetHeight());

	base::BeforeRender();
}