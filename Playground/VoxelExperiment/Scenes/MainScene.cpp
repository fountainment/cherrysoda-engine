#include "MainScene.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Math.h>

#include "VoxelExperiment.h"
#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::Renderer;

class VoxelRenderer : public EverythingRenderer
{
public:
	typedef EverythingRenderer base;

	VoxelRenderer()
	{
		m_mtx = Math::IdentityMat4();
		m_uniformMtx = Graphics::CreateUniformMat4("u_mtx");
	}

	void Render(cherrysoda::Scene* scene) override
	{
		Graphics::Instance()->RenderPass(1);
		Graphics::Instance()->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
		Graphics::Instance()->SetClearColor(cherrysoda::Color::Red);

		Graphics::SetSamplerTexCube(&GameApp::ms_texCube);
		const Math::Mat4 orientationMatrix = Math::GetOrientationMatrix(*GetCamera()->GetViewMatrix());
		Graphics::SetUniform(m_uniformMtx, &orientationMatrix);

		cherrysoda::Camera camera(1.f, 1.f);
		camera.Position(Math::Vec3(0.5f, 0.5f, 0.5f));
		camera.FOV(90.f);
		Graphics::Instance()->SetCamera(&camera);
		Graphics::Instance()->ScreenSpaceQuad(1.f, 1.f);
		Graphics::Instance()->Submit(&GameApp::ms_skyboxShader);

		Graphics::Instance()->RenderPass(2);
		Graphics::Instance()->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
		Graphics::Instance()->SetClearDiscard();
		Graphics::SetSamplerTexCube(&GameApp::ms_texCube);
		Graphics::SetSamplerTexCubeIrr(&GameApp::ms_texCubeIrr);

		Graphics::SetEffect(&GameApp::ms_voxelShader);
		Graphics::Instance()->SetCamera(GetCamera());
		scene->Entities()->Render();
		Graphics::SetEffect(nullptr);
		Graphics::Instance()->RenderPass(0);
	}

	void Update(cherrysoda::Scene* scene) override
	{
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), Engine::Instance()->DeltaTime() * -0.5f, Vec3_YUp));
	}

private:
	Math::Mat4 m_mtx;
	Graphics::UniformHandle m_uniformMtx;
};

void MainScene::Begin()
{
	base::Begin();

	m_renderer = new VoxelRenderer;
	m_chunk = new Chunk;

	m_renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 30.f));
	
	Graphics::SetUniformCamPos(m_renderer->GetCamera()->Position());

	// Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93, 0.88f), 1.f, 0.5f, 0.f);
	// Graphics::SetUniformMaterial(Math::Vec3(0.0277f), 0.1f, 1.f, 0.f);
	Graphics::SetUniformMaterial(Math::Vec3(1.f, 0.72f, 0.29f), 1.0f, 0.99f, 0.f);

	Graphics::SetUniformLight(0, Math::Vec3(-5.f, 5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(1, Math::Vec3(5.f, 5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(2, Math::Vec3(-5.f, -5.f, 8.f), Math::Vec3(1.f));
	Graphics::SetUniformLight(3, Math::Vec3(5.f, -5.f, 8.f), Math::Vec3(1.f));

	Add(m_renderer);
	Add(m_chunk);
}

void MainScene::BeforeRender()
{
	m_renderer->GetCamera()->Width(static_cast<float>(Engine::Instance()->GetWidth()));
	m_renderer->GetCamera()->Height(static_cast<float>(Engine::Instance()->GetHeight()));

	base::BeforeRender();
}