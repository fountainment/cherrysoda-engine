#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using main::MainScene;

static const BitTag s_backgroundTag("background");
static const BitTag s_screenTextureTag("screen_texture");

constexpr int kBackgroundPass = 1;
constexpr int kMainPass = 2;
constexpr int kScreenTexturePass = 3;

class HollowRectGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(HollowRectGraphicsComponent, GraphicsComponent);

	HollowRectGraphicsComponent() : base(true) {}

	void Render() override
	{
		Draw::HollowRect(RenderPosition().x, RenderPosition().y, 12, 16, GetColor());
	}

	void Update() override
	{
		float deltaTime = Engine::Instance()->DeltaTime();
		GetEntity()->MovePositionX(MInput::GamePads(0)->GetLeftStick(0.2f).x * 100.f * deltaTime);
		if (MInput::GamePads(0)->Pressed(Buttons::A) && m_jumpAbility) {
			Jump();
			m_jumpAbility--;
		}
		GetEntity()->MovePositionY(m_upSpeed * deltaTime);
		m_upSpeed -= 500.f * deltaTime;
		if (GetEntity()->PositionY() < 0.f) {
			GetEntity()->PositionY(0.f);
			m_upSpeed = 0.f;
			m_jumpAbility = 2;
		}
	}

	void Jump()
	{
		m_upSpeed = 200.f;
	}

private:
	int m_jumpAbility = 2;
	float m_upSpeed = 0.f;
};

class ScreenSpaceQuadGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(ScreenSpaceQuadGraphicsComponent, GraphicsComponent);

	ScreenSpaceQuadGraphicsComponent(const Texture2D& texture)
		: base(false)
		, m_texture(texture)
	{}

	void Render() override
	{
		Graphics::SetTexture(&m_texture);
		Graphics::ScreenSpaceQuad(m_texture.Width(), m_texture.Height(), Graphics::IsOriginBottomLeft());
		Graphics::SetStateNoDepth();
		Graphics::SubmitOnCurrentRenderPass();
	}

private:
	Texture2D m_texture;
};

void MainScene::Begin()
{
	Graphics::SetPointTextureSampling();
	Graphics::SetRenderPassOrder({ 0, kBackgroundPass, kMainPass, kScreenTexturePass });

	Graphics::UseRenderPass(kBackgroundPass)->SetClearColor(Color::Black);
	Graphics::UseRenderPass(kMainPass)->SetClearDiscard();
	Graphics::UseRenderPass(kScreenTexturePass)->SetClearDiscard();

	m_mainScreenTarget = new RenderTarget2D(320.0f, 180.0f);

	m_backgroundRenderer = new SingleTagRenderer(s_backgroundTag);
	m_mainRenderer = new TagExcludeRenderer(s_backgroundTag | s_screenTextureTag);
	m_screenTexRenderer = new SingleTagRenderer(s_screenTextureTag);

	m_backgroundRenderer->RenderPass(kBackgroundPass);
	m_mainRenderer->RenderPass(kMainPass);
	m_screenTexRenderer->RenderPass(kScreenTexturePass);

	m_backgroundRenderer->SetRenderTarget(m_mainScreenTarget);
	m_mainRenderer->SetRenderTarget(m_mainScreenTarget);

	m_backgroundRenderer->SetEffect(Effect::LoadEffectFromFile("background"));
	m_mainRenderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	m_screenTexRenderer->SetEffect(Effect::LoadEffectFromFile("screenspacequad"));

	m_backgroundRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 1.f));
	m_backgroundRenderer->GetCamera()->UseOrthoProjection(true);

	m_mainRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 1.f));
	m_mainRenderer->GetCamera()->UseOrthoProjection(true);

	m_screenTexRenderer->GetCamera()->Position(Math::Vec3(m_mainScreenTarget->Width() * 0.5f, m_mainScreenTarget->Height() * 0.5f, 1.f));
	m_screenTexRenderer->GetCamera()->UseOrthoProjection(true);
	m_screenTexRenderer->GetCamera()->CenterOrigin();
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(3.f));
	m_screenTexRenderer->KeepCameraCenterOrigin(true);

	Add(m_backgroundRenderer);
	Add(m_mainRenderer);
	Add(m_screenTexRenderer);

	auto backgroundEntity = new Entity();
	backgroundEntity->Add(new ScreenSpaceQuadGraphicsComponent(Texture2D::ForColorBuffer(320.f, 180.f)));
	backgroundEntity->Tag(s_backgroundTag);
	Add(backgroundEntity);

	auto rectEntity = new Entity();
	rectEntity->Add(new HollowRectGraphicsComponent);
	rectEntity->SetCollider(new Hitbox(12, 16));
	Add(rectEntity);

	auto screenTex = new Entity();
	auto screenSpaceQuad = new ScreenSpaceQuadGraphicsComponent(m_mainScreenTarget->GetTexture2D());
	screenTex->Tag(s_screenTextureTag);
	screenTex->Add(screenSpaceQuad);
	Add(screenTex);

	base::Begin();
}

void MainScene::Update()
{
	auto viewSize = Engine::Instance()->GetViewSize();
	int cameraScale = Math_Min(Math_Max(1.f, viewSize.x / (float)m_mainScreenTarget->Width()), \
		Math_Max(1.f, viewSize.y / (float)m_mainScreenTarget->Height()));
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(cameraScale));

	base::Update();
}
