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
#include <CherrySoda/Renderers/SingleTagRenderer.h>
#include <CherrySoda/Util/BitTag.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include "Program.h"
#include "Scenes/Skybox.h"
#include "Voxel/Chunk.h"
#include "Voxel/World.h"

using cherrysoda::BitTag;
using cherrysoda::Component;
using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::SingleTagRenderer;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::MInput;
using cherrysoda::Renderer;
using cherrysoda::STL;
using cherrysoda::StringUtil;

class SkyboxRenderer : public SingleTagRenderer
{
public:
	typedef SingleTagRenderer base;

	SkyboxRenderer(const BitTag& tag)
	: base(tag)
	{
		SetEffect(GameApp::ms_skyboxShader);
	}

	void Render(cherrysoda::Scene* scene) override
	{
		Graphics::Instance()->RenderPass(1);
		Graphics::Instance()->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
		Graphics::Instance()->SetClearDiscard();

		base::Render(scene);
		Graphics::Instance()->RenderPass(0);
	}
};

class VoxelRenderer : public SingleTagRenderer
{
public:
	typedef SingleTagRenderer base;

	VoxelRenderer(const BitTag& tag)
	: base(tag)
	{
		SetEffect(GameApp::ms_voxelShader);
	}

	void Render(cherrysoda::Scene* scene) override
	{
		Graphics::Instance()->RenderPass(2);
		Graphics::Instance()->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
		Graphics::Instance()->SetClearDiscard();

		base::Render(scene);
		Graphics::Instance()->RenderPass(0);
	}

	void Update(cherrysoda::Scene* scene) override
	{
		float deltaTime = Engine::Instance()->DeltaTime();
		Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick(0.01f);
		Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick(0.01f);
		leftStick *= Math_Length(leftStick) * 40.0f;
		rightStick *= Math_LengthSq(rightStick) * 2.0f;
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), deltaTime * rightStick[0], GetCamera()->GetUpVector()));
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), deltaTime * rightStick[1], GetCamera()->GetLeftVector()));
		GetCamera()->Position(GetCamera()->Position() + deltaTime * GetCamera()->GetFrontVector() * leftStick[1]);
		GetCamera()->Position(GetCamera()->Position() + deltaTime * GetCamera()->GetRightVector() * leftStick[0]);

		// CHERRYSODA_DEBUG(StringUtil::Format("%f %f %f %f\n", leftStick[0], leftStick[1], rightStick[0], rightStick[1]));
	}

};

void MainScene::Begin()
{
	base::Begin();

	m_uniformMtx = Graphics::CreateUniformMat4("u_mtx");
	Graphics::Instance()->SetRenderPassOrder({ 0, 1, 2 });

	ms_skyboxTag = BitTag("skybox");
	ms_voxelTag = BitTag("voxel");
	m_skyboxRenderer = new SkyboxRenderer(ms_skyboxTag);
	m_voxelRenderer = new VoxelRenderer(ms_voxelTag);

	m_skybox = new Entity;
	m_skybox->Add(new Skybox);
	m_skybox->AddTag(ms_skyboxTag);

	m_chunk = new Chunk;
	m_chunk->AddTag(ms_voxelTag);

	m_chunk1 = new Chunk;
	m_chunk1->Position(Math::Vec3(0.f, 0.f, -30.f));
	m_chunk1->AddTag(ms_voxelTag);

	m_voxelWorld = new World;
	Chunk* chunks = m_voxelWorld->LoadChunks();
	for (int i = 0; i < World::ChunksAmount(); ++i) {
		// chunks[i].FillAllBlocks(Block::Type::White);
		chunks[i].AddTag(ms_voxelTag);
		Add(chunks + i);
	}

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
				if (!haveBlock) {
					m_chunk1->SetBlockType(i, j, k, Block::Type::White);
				}
			}
		}
	}

	m_skyboxRenderer->GetCamera()->Width(1);
	m_skyboxRenderer->GetCamera()->Height(1);
	m_skyboxRenderer->GetCamera()->Position(Math::Vec3(0.5f, 0.5f, 0.5f));
	m_skyboxRenderer->GetCamera()->FOV(90.f);
	m_voxelRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 30.f));

	Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93, 0.88f), 1.f, 0.5f, 0.f); // Silver
	// Graphics::SetUniformMaterial(Math::Vec3(0.0277f), 0.1f, 1.f, 0.f); 	
	// Graphics::SetUniformMaterial(Math::Vec3(1.f, 0.72f, 0.29f), 1.0f, 0.99f, 0.f); // Gold

	// Graphics::SetUniformLight(0, Math::Vec3(-5.f, 5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(1, Math::Vec3(5.f, 5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(2, Math::Vec3(-5.f, -5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(3, Math::Vec3(5.f, -5.f, 8.f), Math::Vec3(1.f));

	Graphics::SetUniformLight(0, Vec3_Zero, Vec3_Zero);
	Graphics::SetUniformLight(1, Vec3_Zero, Vec3_Zero);
	Graphics::SetUniformLight(2, Vec3_Zero, Vec3_Zero);
	Graphics::SetUniformLight(3, Vec3_Zero, Vec3_Zero);

	// Renderers
	Add(m_skyboxRenderer);
	Add(m_voxelRenderer);

	// Entities
	Add(m_chunk);
	Add(m_chunk1);
	Add(m_skybox);
}

void MainScene::BeforeRender()
{
	m_voxelRenderer->GetCamera()->Width(static_cast<float>(Engine::Instance()->GetWidth()));
	m_voxelRenderer->GetCamera()->Height(static_cast<float>(Engine::Instance()->GetHeight()));

	Graphics::SetUniformCamPos(m_voxelRenderer->GetCamera()->Position());
	const Math::Mat4 orientationMatrix = Math::GetOrientationMatrix(*m_voxelRenderer->GetCamera()->GetViewMatrix());
	Graphics::SetUniform(m_uniformMtx, &orientationMatrix);

	base::BeforeRender();
}

void MainScene::Update()
{
	base::Update();

	static int r = 0;

	if (r < World::BlocksAmount())
	{
		for (int i = 0; i < 1000; ++i)
		{
			m_voxelWorld->SetBlockType(r, Block::Type::White);
			r += 29;
		}
	}
}

BitTag MainScene::ms_skyboxTag;
BitTag MainScene::ms_voxelTag;