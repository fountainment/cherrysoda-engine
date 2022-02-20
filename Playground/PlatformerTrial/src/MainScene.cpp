#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;
using main::MainScene;

static const BitTag s_backgroundTag("background");
static const BitTag s_screenTextureTag("screen_texture");

static Pool<Hitbox, 1024> s_hitboxPool;

constexpr type::UInt16 kBackgroundPass = 1;
constexpr type::UInt16 kMainPass = 2;
constexpr type::UInt16 kScreenTexturePass = 3;

class alignas(64) Simple2DPhysicsComponent : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Simple2DPhysicsComponent, Component);

	static const BitTag StaticTag;

	Simple2DPhysicsComponent()
	: base(true, false)
	{}

	void Added(Entity* entity) override
	{
		base::Added(entity);

		entity->AddTag(DynamicTag);
	}

	inline void Move(const Math::Vec2& v)
	{
		m_pendingMove += v;
	}

	inline bool IsOnGround() const
	{
		return m_isOnGround;
	}

	inline bool IsAnythingAbove() const
	{
		return m_isAnythingAbove;
	}

	Math::Vec2 ActualMove(bool passive = false)
	{
		if (passive) {
			m_pendingMove *= m_pushability;
		}
		m_remainder += m_pendingMove;
		m_pendingMove = Vec2_Zero;

		if (Math_Abs(m_remainder.x) == 0.5f) m_remainder.x -= Math_Sign(m_remainder.x) * Math::epsf;
		if (Math_Abs(m_remainder.y) == 0.5f) m_remainder.y -= Math_Sign(m_remainder.y) * Math::epsf;

		int moveX = (int)Math_Round(m_remainder.x);
		int moveY = (int)Math_Round(m_remainder.y);

		auto entity = GetEntity();

		Math::Vec2 oldPosition = entity->Position2D();
		Math::Vec2 actualMove(moveX, moveY);

		if (moveX != 0 || moveY != 0) {

			entity->Collidable(false);

			if (moveX != 0) {
				m_remainder.x -= moveX;
				entity->MovePositionX(moveX);

				bool collideWithStatics = false;

				for (auto staticEntity : (*GetScene())[StaticTag]) {
					if (entity->CollideCheck(staticEntity)) {
						if (moveX > 0) {
							entity->Right(staticEntity->Left());
						}
						else {
							entity->Left(staticEntity->Right());
						}
						float deltaX = entity->PositionX() - oldPosition.x;
						actualMove.x = Math_Abs(deltaX) < Math_Abs(actualMove.x) ? deltaX : actualMove.x;

						collideWithStatics = true;
					}
				}

				if (!collideWithStatics) {
					for (auto dynamicEntity : (*GetScene())[DynamicTag]) {
						if (entity->CollideCheck(dynamicEntity)) {
							auto dynamic = dynamicEntity->Get<Simple2DPhysicsComponent>();
							Math::Vec2 oldDynamicPendingMove = dynamic->m_pendingMove;
							float pushMoveX = moveX > 0 ? entity->Right() - dynamicEntity->Left() : entity->Left() - dynamicEntity->Right();
							dynamic->m_pendingMove = Math::Vec2(pushMoveX, 0.f);
							Math::Vec2 dynamicActualMove = dynamic->ActualMove(true);
							dynamic->m_pendingMove = oldDynamicPendingMove;

							float deltaX = dynamicActualMove.x + moveX - pushMoveX;
							actualMove.x = Math_Abs(deltaX) < Math_Abs(actualMove.x) ? deltaX : actualMove.x;
						}
					}
					entity->MovePositionX(actualMove.x - moveX);
				}
			}

			if (moveY != 0) {
				m_remainder.y -= moveY;
				entity->MovePositionY(moveY);

				bool collideWithStatics = false;

				for (auto staticEntity : (*GetScene())[StaticTag]) {
					if (entity->CollideCheck(staticEntity)) {
						if (moveY > 0) {
							entity->Top(staticEntity->Bottom());
						}
						else {
							entity->Bottom(staticEntity->Top());
						}
						float deltaY = entity->PositionY() - oldPosition.y;
						actualMove.y = Math_Abs(deltaY) < Math_Abs(actualMove.y) ? deltaY : actualMove.y;

						collideWithStatics = true;
					}
				}

				if (!collideWithStatics) {
					for (auto dynamicEntity : (*GetScene())[DynamicTag]) {
						if (entity->CollideCheck(dynamicEntity)) {
							auto dynamic = dynamicEntity->Get<Simple2DPhysicsComponent>();
							Math::Vec2 oldDynamicPendingMove = dynamic->m_pendingMove;
							float pushMoveY = moveY > 0 ? entity->Top() - dynamicEntity->Bottom() : entity->Bottom() - dynamicEntity->Top();
							dynamic->m_pendingMove = Math::Vec2(0.f, pushMoveY);
							Math::Vec2 dynamicActualMove = dynamic->ActualMove(true);
							dynamic->m_pendingMove = oldDynamicPendingMove;

							float deltaY = dynamicActualMove.y + moveY - pushMoveY;
							actualMove.y = Math_Abs(deltaY) < Math_Abs(actualMove.y) ? deltaY : actualMove.y;
						}
					}
					entity->MovePositionY(actualMove.y - moveY);
				}
			}
		}

		m_isOnGround = false;
		if (entity->CollideCheck(StaticTag, entity->Position2D() - Vec2_YUp) || entity->CollideCheck(DynamicTag, entity->Position2D() - Vec2_YUp)) {
			m_isOnGround = true;
		}
		m_isAnythingAbove = false;
		if (entity->CollideCheck(StaticTag, entity->Position2D() + Vec2_YUp) || entity->CollideCheck(DynamicTag, entity->Position2D() + Vec2_YUp)) {
			m_isAnythingAbove = true;
		}

		entity->Collidable(true);

		return actualMove;
	}

private:
	static const BitTag DynamicTag;

	Math::Vec2 m_pendingMove = Vec2_Zero;
	Math::Vec2 m_remainder = Vec2_Zero;
	bool m_isOnGround = false;
	bool m_isAnythingAbove = false;

	float m_pushability = 0.5f;
};

const BitTag Simple2DPhysicsComponent::StaticTag("SPC_static");
const BitTag Simple2DPhysicsComponent::DynamicTag("SPC_dynamic");

static Pool<Simple2DPhysicsComponent, 100> s_physCompPool;

class HollowRectGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(HollowRectGraphicsComponent, GraphicsComponent);

	HollowRectGraphicsComponent(int width, int height)
	: base(false)
	, m_width(width)
	, m_height(height)
	{}

	void Render() override
	{
		Draw::HollowRect(RenderPosition().x, RenderPosition().y, m_width, m_height, GetColor());
	}

private:
	int m_width;
	int m_height;
};

class PlayerControl : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(PlayerControl, Component);

	PlayerControl() : base(true, false) {}

	void Update() override
	{
		auto entity = GetEntity();
		auto physComp = entity->Get<Simple2DPhysicsComponent>();
		float deltaTime = Engine::Instance()->DeltaTime();
		bool jumpButtonPressed = MInput::GamePads(0)->Pressed(Buttons::A) || MInput::Keyboard()->Pressed(Keys::Space);
		bool jumpButtonCheck = MInput::GamePads(0)->Check(Buttons::A) || MInput::Keyboard()->Check(Keys::Space);
		float extraDropSpeed = -Math_Min(MInput::GamePads(0)->GetLeftStick().y, 0.f) * 300.f;
		if (MInput::Keyboard()->Check(Keys::S)) extraDropSpeed = 300.f;
		if (jumpButtonCheck) {
			extraDropSpeed -= 400.f;
		}
		if (physComp->IsOnGround()) {
			m_speedY = 0.f;
			if (jumpButtonPressed) {
				Jump();
			}
		}
		else {
			m_speedY -= (1000.f + extraDropSpeed) * deltaTime;
		}
		if (physComp->IsAnythingAbove()) {
			if (m_speedY > 0.f) {
				m_speedY = 0.f;
			}
		}
		float speedX = MInput::GamePads(0)->GetLeftStick(0.2f).x * 100.f;
		if (!MInput::GamePads(0)->Attached() || speedX == 0.f) {
			speedX = MInput::Keyboard()->AxisCheck(Keys::A, Keys::D) * 100.f;
		}
		physComp->Move(Math::Vec2(speedX, m_speedY) * deltaTime);
	}

	void Jump()
	{
		m_speedY = 200.f;
	}

private:
	float m_speedY = 0.f;

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
	rectEntity->Position2D(Math::Vec2(0.f, 1.f));
	rectEntity->Add(new HollowRectGraphicsComponent(8, 12));
	rectEntity->Add(new PlayerControl);
	rectEntity->Add(s_physCompPool.Create());
	rectEntity->SetCollider(s_hitboxPool.Create(8, 12));
	Add(rectEntity);

	auto ground = new Entity();
	ground->Position2D(Math::Vec2(0.f, 0.f));
	ground->Add(new HollowRectGraphicsComponent(320, 1));
	ground->SetCollider(s_hitboxPool.Create(320, 1));
	ground->Tag(Simple2DPhysicsComponent::StaticTag);
	Add(ground);

	auto wall = new Entity();
	wall->Position2D(Math::Vec2(50.f, 35.f));
	wall->Add(new HollowRectGraphicsComponent(10, 10));
	wall->SetCollider(s_hitboxPool.Create(10, 10));
	wall->Tag(Simple2DPhysicsComponent::StaticTag);
	Add(wall);

	auto brick = new Entity();
	brick->Position2D(Math::Vec2(150.f, 1.f));
	brick->Add(new HollowRectGraphicsComponent(10, 10));
	brick->Add(s_physCompPool.Create());
	brick->SetCollider(s_hitboxPool.Create(10, 10));
	Add(brick);

	auto screenTex = new Entity();
	auto screenSpaceQuad = new ScreenSpaceQuadGraphicsComponent(m_mainScreenTarget->GetTexture2D());
	screenTex->Tag(s_screenTextureTag);
	screenTex->Add(screenSpaceQuad);
	Add(screenTex);

	base::Begin();
}

void MainScene::Update()
{
	s_physCompPool.Traverse([](Simple2DPhysicsComponent* ptr) { ptr->ActualMove(); });

	auto viewSize = Engine::Instance()->GetViewSize();
	int cameraScale = Math_Min(Math_Max(1.f, viewSize.x / (float)m_mainScreenTarget->Width()), \
		Math_Max(1.f, viewSize.y / (float)m_mainScreenTarget->Height()));
	m_screenTexRenderer->GetCamera()->Scale2D(Math::Vec2(cameraScale));

	base::Update();
}
