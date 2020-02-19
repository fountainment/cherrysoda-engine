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
		Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick(0.2f);
		Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick(0.2f);
		leftStick *= Math_Length(leftStick) * 40.0f;
		rightStick *= Math_LengthSq(rightStick) * 2.0f;
		GetCamera()->Direction(Math::RotateVector(GetCamera()->Direction(), deltaTime * rightStick[0], GetCamera()->GetUpVector()));
		Math::Vec3 targetDirection = Math::RotateVector(GetCamera()->Direction(), deltaTime * rightStick[1], GetCamera()->GetLeftVector());
		if (Math_Dot(Math_Normalize(Math_Cross(GetCamera()->GetUpVector(), targetDirection)), GetCamera()->GetLeftVector()) > 0.0f)
		{
			 GetCamera()->Direction(targetDirection);
		}
		GetCamera()->Position(GetCamera()->Position() + deltaTime * GetCamera()->GetFrontVector() * leftStick[1]);
		GetCamera()->Position(GetCamera()->Position() + deltaTime * GetCamera()->GetRightVector() * leftStick[0]);

		// CHERRYSODA_DEBUG_FORMAT("%f %f %f %f\n", leftStick[0], leftStick[1], rightStick[0], rightStick[1]);
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

	Math::Vec3 worldBasePosition(-World::Size() * Chunk::Size() / 2.0f);
	m_voxelWorld = new World(worldBasePosition);
	Chunk* chunks = m_voxelWorld->LoadChunks();
	for (int i = 0; i < World::ChunkAmount(); ++i) {
		chunks[i].FillAllBlocks(Block::Type::Black);
		for (int z = 0; z < Chunk::Size(); ++z) {
			for (int y = 0; y < Chunk::Size(); ++y) {
				for (int x = 0; x < Chunk::Size(); ++x) {
					if ((x + y + z) % 2 == 0) {
						chunks[i].SetBlockType(x, y, z, Block::Type::White);
					}
					else {
						chunks[i].SetBlockType(x, y, z, Block::Type::Black);
					}
				}
			}
		}
		chunks[i].AddTag(ms_voxelTag);
		Add(chunks + i);
	}

	m_skyboxRenderer->GetCamera()->Width(1);
	m_skyboxRenderer->GetCamera()->Height(1);
	m_skyboxRenderer->GetCamera()->Position(Math::Vec3(0.5f, 0.5f, 0.5f));
	m_skyboxRenderer->GetCamera()->FOV(90.f);

	m_voxelRenderer->GetCamera()->Position(Math::Vec3(0.f, 70.f, 0.f));

	// Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93, 0.88f), 1.f, 0.5f, 0.f); // Silver
	Graphics::SetUniformMaterial(Math::Vec3(0.0277f), 0.3f, 1.f, 0.4f); 	
	// Graphics::SetUniformMaterial(Math::Vec3(1.f, 0.72f, 0.29f), 1.0f, 0.99f, 0.f); // Gold

	// Graphics::SetUniformLight(0, Math::Vec3(-5.f, 5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(1, Math::Vec3(5.f, 5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(2, Math::Vec3(-5.f, -5.f, 8.f), Math::Vec3(1.f));
	// Graphics::SetUniformLight(3, Math::Vec3(5.f, -5.f, 8.f), Math::Vec3(1.f));

	// Graphics::SetUniformLight(0, Vec3_Zero, Vec3_Zero);
	// Graphics::SetUniformLight(1, Vec3_Zero, Vec3_Zero);
	// Graphics::SetUniformLight(2, Vec3_Zero, Vec3_Zero);
	// Graphics::SetUniformLight(3, Vec3_Zero, Vec3_Zero);

	// Renderers
	Add(m_skyboxRenderer);
	Add(m_voxelRenderer);

	// Entities
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
/*
	int halfWorldBlockSize = World::WorldBlockSize() / 2;

	static float fr = static_cast<float>(1.0f);
	static float dir = 1.0f;
	int r = static_cast<int>(fr);

	if (dir > 0.0f) {
		if (r < halfWorldBlockSize) {
			for (int i = -r; i <= r; ++i) {
				for (float theta = 0.f; theta < 2.0f * 3.14159f; theta += 0.01f) {
					int y = halfWorldBlockSize + i;
					float rr = static_cast<float>(glm::pow(r * r - i * i, 0.5));
					int x = halfWorldBlockSize + static_cast<int>(glm::cos(theta) * rr);
					int z = halfWorldBlockSize + static_cast<int>(glm::sin(theta) * rr);
					m_voxelWorld->SetBlockType(x, y, z, Block::Type::White);
				}
			}
		}
		else {
			dir = -1.0f;
		}
	}
	else {
		if (r > 0) {
			for (int i = -r; i <= r; ++i) {
				for (float theta = 0.f; theta < 2.0f * 3.14159f; theta += 0.01f) {
					int y = halfWorldBlockSize + i;
					float rr = static_cast<float>(glm::pow(r * r - i * i, 0.5));
					int x = halfWorldBlockSize + static_cast<int>(glm::cos(theta) * rr);
					int z = halfWorldBlockSize + static_cast<int>(glm::sin(theta) * rr);
					m_voxelWorld->SetBlockType(x, y, z, Block::Type::None);
				}
			}
		}
		else {
			dir = 1.0f;
		}
	}
	fr += 0.016667f * 20.0f * dir * (Math_Abs(glm::sin(fr / halfWorldBlockSize * 3.14159f)) + 0.1f) * 2.0f;
*/
	base::Update();
}

BitTag MainScene::ms_skyboxTag;
BitTag MainScene::ms_voxelTag;