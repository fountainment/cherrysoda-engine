#include "MainScene.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Input/MInput.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

#include "VoxelExperiment.h"
#include "Voxel/Chunk.h"

using cherrysoda::Component;
using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::EverythingRenderer;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::MInput;
using cherrysoda::Renderer;
using cherrysoda::StringUtil;

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

		cherrysoda::Camera camera(1, 1);
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
		float deltaTime = Engine::Instance()->DeltaTime();
		Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick();
		Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick();
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), 1.5f * deltaTime * rightStick[0], Vec3_YUp));
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), 1.5f * deltaTime * rightStick[1], GetCamera()->GetLeftVector()));
		GetCamera()->Position(GetCamera()->Position() + 30.0f * deltaTime * GetCamera()->GetFrontVector() * leftStick[1]);
		GetCamera()->Position(GetCamera()->Position() + 30.0f * deltaTime * GetCamera()->GetRightVector() * leftStick[0]);

		Graphics::SetUniformCamPos(GetCamera()->Position());
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

	constexpr int chunkSize = Chunk::Size();
	auto onEdge = [chunkSize](int x) { return x == 0 || x == chunkSize - 1; };
	auto onCross = [chunkSize](int x, int y) { return x == y || x == chunkSize - y; };
	constexpr float halfChunkSize = chunkSize * 0.5f;
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				/*
				float l = i - halfChunkSize + 0.5f;
				float m = j - halfChunkSize + 0.5f;
				float n = k - halfChunkSize + 0.5f;
				if ((l * l + m * m + n * n) <= halfChunkSize * halfChunkSize) {
					m_chunk->SetBlockType(i, j, k, Block::Type::White);
				}
				*/
				bool haveBlock = false;
				haveBlock |= onEdge(i) && onEdge(j);
				haveBlock |= onEdge(i) && onEdge(k);
				haveBlock |= onEdge(j) && onEdge(k);

				if (haveBlock) {
					m_chunk->SetBlockType(i, j, k, Block::Type::White);
				}
			}
		}
	}

	m_renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 30.f));
	
	Graphics::SetUniformCamPos(m_renderer->GetCamera()->Position());

	Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93, 0.88f), 1.f, 0.5f, 0.f); // Silver
	// Graphics::SetUniformMaterial(Math::Vec3(0.0277f), 0.1f, 1.f, 0.f); 	
	// Graphics::SetUniformMaterial(Math::Vec3(1.f, 0.72f, 0.29f), 1.0f, 0.99f, 0.f); // Gold

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