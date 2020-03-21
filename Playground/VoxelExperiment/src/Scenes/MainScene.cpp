#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

#include "Program.h"
#include "Scenes/Skybox.h"
#include "Voxel/Chunk.h"
#include "Voxel/World.h"

using namespace cherrysoda;

class SkyboxRenderer : public SingleTagRenderer
{
public:
	typedef SingleTagRenderer base;

	SkyboxRenderer(const BitTag& tag)
	: base(tag)
	{
		SetEffect(GameApp::ms_skyboxShader);
	}

	void BeforeRender(Scene* scene) override
	{
		Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
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

	void BeforeRender(Scene* scene) override
	{
		Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight());
	}

	void Update(Scene* scene) override
	{
		float deltaTime = Engine::Instance()->DeltaTime();
		Math::Vec2 leftStick = MInput::GamePads(0)->GetLeftStick(0.2f);
		Math::Vec2 rightStick = MInput::GamePads(0)->GetRightStick(0.2f);
		leftStick *= Math_Length(leftStick) * 40.0f;
		rightStick *= Math_LengthSq(rightStick) * Math::Pi;
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
	
	Graphics::SetRenderPassOrder({ 0, 1, 2 });
	Graphics::UseRenderPass(1)->SetClearDiscard();
	Graphics::UseRenderPass(2)->SetClearDiscard();

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
		for (int z = 0; z < Chunk::Size(); ++z) {
			for (int y = 0; y < Chunk::Size(); ++y) {
				for (int x = 0; x < Chunk::Size(); ++x) {
					Math::IVec3 blockIndex(x, y, z);
					if ((x + y + z) % 2 == 0) {
						chunks[i].SetBlockType(blockIndex, Block::Type::White);
					}
					else {
						chunks[i].SetBlockType(blockIndex, Block::Type::Black);
					}
				}
			}
		}
		chunks[i].AddTag(ms_voxelTag);
		Add(chunks + i);
	}

	m_skyboxRenderer->RenderPass(1);
	m_skyboxRenderer->GetCamera()->Width(1);
	m_skyboxRenderer->GetCamera()->Height(1);
	m_skyboxRenderer->GetCamera()->Position(Math::Vec3(0.5f, 0.5f, 0.5f));
	m_skyboxRenderer->GetCamera()->FOV(90.f);

	m_voxelRenderer->RenderPass(2);
	m_voxelRenderer->GetCamera()->Position(Math::Vec3(0.f, 70.f, 0.f));

	// Graphics::SetUniformMaterial(Math::Vec3(0.95f, 0.93f, 0.88f), 1.f, 0.f, 0.f); // Silver
	Graphics::SetUniformMaterial(Math::Vec3(0.08f), 0.3f, 0.1f, 0.f); // Glass
	// Graphics::SetUniformMaterial(Math::Vec3(1.f, 0.72f, 0.29f), 1.0f, 0.f, 0.f); // Gold

	Graphics::SetUniformLight(0, Math::Vec3(  0.f, 68.f,   0.f), Vec3_One * 5.f, false);
	Graphics::SetUniformLight(1, Math::Vec3( 30.f, 68.f,   0.f), Vec3_XUp * 5.f, false);
	Graphics::SetUniformLight(2, Math::Vec3(-15.f, 68.f, -26.f), Vec3_YUp * 5.f, false);
	Graphics::SetUniformLight(3, Math::Vec3(-15.f, 68.f,  26.f), Vec3_ZUp * 5.f, false);
	Graphics::SubmitUniformLight();

	// Renderers
	Add(m_skyboxRenderer);
	Add(m_voxelRenderer);

	// Entities
	Add(m_skybox);
}

void MainScene::BeforeRender()
{
	m_voxelRenderer->GetCamera()->SetSize(Engine::Instance()->GetViewSize());

	Graphics::SetUniformCamPos(m_voxelRenderer->GetCamera()->Position());
	const Math::Mat4 orientationMatrix = Math::GetOrientationMatrix(*m_voxelRenderer->GetCamera()->GetViewMatrix());
	Graphics::SetUniform(m_uniformMtx, &orientationMatrix);

	base::BeforeRender();
}

void MainScene::Update()
{
	Camera* camera = m_voxelRenderer->GetCamera();
	float t;
	Math::Vec3 pos = camera->Position();
	Math::Vec3 direction = camera->Direction();
	if (Math_RaycastAABB(pos, direction, m_voxelWorld->GetAABB(), &t)) {
		for (;;) {
			pos = pos + direction * (t + 0.1f);
			auto index = m_voxelWorld->GetIndexOfBlockAt(pos);
			auto block = m_voxelWorld->GetBlock(index);
			m_voxelWorld->SetBlockType(index, Block::Type::None);
			if (!block || !Math_RaycastAABB(pos, direction, m_voxelWorld->GetBlockAABB(index), nullptr, &t)) {
				break;
			}
		}
	}

	static float s_t = 0.f;
	s_t += Engine::Instance()->DeltaTime();
	Graphics::SetUniformLight(1, Math::Vec3(Math_Cos(s_t) * 30.0f, 68.f, Math_Sin(s_t) * 30.0f), Vec3_XUp * 5.f, false);
	Graphics::SetUniformLight(2, Math::Vec3(Math_Cos(s_t + Math::Pi2 / 3.f) * 30.0f, 68.f, Math_Sin(s_t + Math::Pi2 / 3.f) * 30.0f), Vec3_YUp * 5.f, false);
	Graphics::SetUniformLight(3, Math::Vec3(Math_Cos(s_t - Math::Pi2 / 3.f) * 30.0f, 68.f, Math_Sin(s_t - Math::Pi2 / 3.f) * 30.0f), Vec3_ZUp * 5.f, false);
	Graphics::SubmitUniformLight();

	base::Update();
}

BitTag MainScene::ms_skyboxTag;
BitTag MainScene::ms_voxelTag;
