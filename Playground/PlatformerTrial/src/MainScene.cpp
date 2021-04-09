#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using main::MainScene;

static const BitTag s_texture("screen_texture");

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

	m_screenTexRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 1.f));
	m_screenTexRenderer->GetCamera()->UseOrthoProjection(true);
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(3.f));
	m_screenTexRenderer->KeepCameraCenterOrigin(true);

	Add(m_mainRenderer);
	Add(m_screenTexRenderer);

	auto screenTex = new Entity();
	auto image = new Image(MTexture(m_mainScreenTarget->GetTexture2D()));
	image->CenterOrigin();
	if (Graphics::IsOriginBottomLeft()) {
		image->SetSpriteEffects(SpriteEffects::FlipVertically);
	}
	screenTex->Tag(s_texture);
	screenTex->Add(image);

	Add(screenTex);

	base::Begin();
}

void MainScene::Update()
{
	auto viewSize = Engine::Instance()->GetViewSize();
	int cameraScale = Math_Min(Math_Max(1.f, viewSize.x / 320.f), Math_Max(1.f, viewSize.y / 180.f));
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(cameraScale));

	base::Update();
}
