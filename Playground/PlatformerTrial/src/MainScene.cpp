#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using main::MainScene;

static const BitTag s_triangle("triangle");
static const BitTag s_texture("texture");

class Triangle : public MeshGraphicsComponent<Graphics::PosColorVertex>
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Triangle, MeshGraphicsComponent<VertexType>);

	Triangle() : base(true)
	{
		GetMesh()->Clear();
		GetMesh()->AddTriangle(
			MK_VERT(Math::Vec3( 0.0f,   1.0f, 0.0f), Color::Red),
			MK_VERT(Math::Vec3(-0.87f, -0.5f, 0.0f), Color::Green),
			MK_VERT(Math::Vec3( 0.87f, -0.5f, 0.0f), Color::Blue)
		);
		GetMesh()->SubmitBuffer();
	}

	void Update() override
	{
		// RotateOnZ(Engine::Instance()->DeltaTime());
	}
};

void MainScene::Begin()
{
	// Add scene initialization here
	Graphics::SetPointTextureSampling();
	Graphics::SetRenderPassOrder({ 0, 1, 2 });

	Graphics::UseRenderPass(1)->SetClearColor(Color::Orange);

	m_mainScreenTarget = new RenderTarget2D(320.0f, 180.0f);
	m_mainRenderer = new SingleTagRenderer(s_triangle);
	m_screenTexRenderer = new SingleTagRenderer(s_texture);

	m_mainRenderer->RenderPass(1);
	m_screenTexRenderer->RenderPass(2);

	m_mainRenderer->SetRenderTarget(m_mainScreenTarget);
	m_screenTexRenderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	m_mainRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 4.f));
	m_screenTexRenderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 175.f));

	Add(m_mainRenderer);
	Add(m_screenTexRenderer);

	auto triangle = new Entity();
	triangle->Tag(s_triangle);
	triangle->Add(new Triangle);

	auto screenTex = new Entity();
	auto image = new Image(MTexture(m_mainScreenTarget->GetTexture2D()));
	image->CenterOrigin();
	if (Graphics::IsOriginBottomLeft()) {
		image->SetSpriteEffects(SpriteEffects::FlipVertically);
	}
	screenTex->Tag(s_texture);
	screenTex->Add(image);

	Add(triangle);
	Add(screenTex);

	base::Begin();
}

void MainScene::Update()
{
	// Add scene update here

	base::Update();
}
