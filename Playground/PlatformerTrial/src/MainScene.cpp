#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using main::MainScene;

static const BitTag s_texture("screen_texture");

class HollowRectGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(HollowRectGraphicsComponent, GraphicsComponent);

	HollowRectGraphicsComponent() : base(false) {}

	void Render() override
	{
		Draw::HollowRect(0, 0, 10, 10);
	}
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
		Graphics::ScreenSpaceQuad(m_texture.Width(), m_texture.Height(), \
			!Graphics::IsOriginBottomLeft(), m_texture.Width(), m_texture.Height());
		Graphics::SetStateNoDepth();
		Graphics::SubmitOnCurrentRenderPass();
	}

private:
	Texture2D m_texture;
};

void MainScene::Begin()
{
	Graphics::SetPointTextureSampling();
	Graphics::SetRenderPassOrder({ 0, 1, 2 });

	Graphics::UseRenderPass(1)->SetClearColor(Color::Orange);
	Graphics::UseRenderPass(2)->SetClearDiscard();

	m_mainScreenTarget = new RenderTarget2D(320.0f, 180.0f);
	m_mainRenderer = new TagExcludeRenderer(s_texture);
	m_screenTexRenderer = new SingleTagRenderer(s_texture);

	m_mainRenderer->RenderPass(1);
	m_screenTexRenderer->RenderPass(2);

	m_mainRenderer->SetRenderTarget(m_mainScreenTarget);
	m_screenTexRenderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	m_mainRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 1.f));
	m_mainRenderer->GetCamera()->UseOrthoProjection(true);

	m_screenTexRenderer->GetCamera()->Position(Math::Vec3(m_mainScreenTarget->Width() * 0.5f, m_mainScreenTarget->Height() * 0.5f, 1.f));
	m_screenTexRenderer->GetCamera()->UseOrthoProjection(true);
	m_screenTexRenderer->GetCamera()->CenterOrigin();
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(3.f));
	m_screenTexRenderer->KeepCameraCenterOrigin(true);

	Add(m_mainRenderer);
	Add(m_screenTexRenderer);

	auto screenTex = new Entity();
	auto screenSpaceQuad = new ScreenSpaceQuadGraphicsComponent(m_mainScreenTarget->GetTexture2D());
	screenTex->Tag(s_texture);
	screenTex->Add(screenSpaceQuad);

	Add(screenTex);

	auto entity = new Entity();
	entity->Add(new HollowRectGraphicsComponent);
	Add(entity);

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
