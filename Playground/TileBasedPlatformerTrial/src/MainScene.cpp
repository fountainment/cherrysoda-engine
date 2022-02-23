#include "MainScene.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using main::MainScene;

static BitTag s_solidTag("solid");
static BitTag s_renderTargetTag("render_target");

CHERRYSODA_CONSOLE_VARIABLE(actor_drop_acceleration, float, 1500.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_extra_drop_acceleration, float, 400.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_jump_speed, float, 360.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_horizontal_move_speed, float, 100.f, "");

static constexpr int kNormalRenderPass = 1;
static constexpr int kFinalRenderPass = 2;

class Actor : public Entity
{
public:
	typedef Entity base;

	void MoveX(float amount)
	{
		m_remainder.x += amount;
		int move = (int)Math_Round((double)m_remainder.x);

		if (move != 0) {
			m_remainder.x -= move;
			int sign = Math_Sign(move);

			while (move != 0) {
				if (!CollideCheck(s_solidTag, Position2D() + Math::Vec2(sign, 0))) {
					MovePositionX(sign);
					move -= sign;
				}
				else {
					break;
				}
			}
		}
	}

	void MoveY(float amount)
	{
		m_remainder.y += amount;
		int move = (int)Math_Round((double)m_remainder.y);

		if (move != 0) {
			m_remainder.y -= move;
			int sign = Math_Sign(move);

			while (move != 0) {
				if (!CollideCheck(s_solidTag, Position2D() + Math::Vec2(0, sign))) {
					MovePositionY(sign);
					move -= sign;
				}
				else {
					break;
				}
			}
		}
	}

	inline void Move(float x, float y) { MoveX(x); MoveY(y); }
	inline void Move(const Math::Vec2& move) { Move(move.x, move.y); }

	void Update() override
	{
		m_isOnGround = false;
		if (CollideCheck(s_solidTag, Position2D() - Vec2_YUp)) {
			m_isOnGround = true;
		}
		m_isAnythingAbove = false;
		if (CollideCheck(s_solidTag, Position2D() + Vec2_YUp)) {
			m_isAnythingAbove = true;
		}

		base::Update();
	}

	inline bool IsOnGround() const { return m_isOnGround; }
	inline bool IsAnythingAbove() const { return m_isAnythingAbove; }

private:
	Math::Vec2 m_remainder;
	bool m_isOnGround = false;
	bool m_isAnythingAbove = false;
};

class CameraFollow : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CameraFollow, Component);

	CameraFollow(Camera* camera)
		: base(true, false)
		, m_camera(camera)
	{}

	void Update() override
	{
		m_camera->Approach(GetEntity()->Position2D(), Math::Rectangle{ Math::Vec2(-50.f, -30.f), Math::Vec2(100.f, 60.f) }, 0.1f);
		m_camera->RoundPosition();
	}

private:
	Camera* m_camera = nullptr;
};

class CameraRestrict : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CameraRestrict, Component);

	CameraRestrict(Camera* camera, float left, float right, float bottom, float top)
		: base(true, false)
		, m_camera(camera)
	{
		m_rect = Math::Rectangle{ Math::Vec2(left, bottom), Math::Vec2(right - left, top - bottom) };
	}

	void Update() override
	{
		float rd = m_rect.Right() - m_camera->Right();
		if (rd < 0.f) m_camera->MovePositionX(rd);
		float td = m_rect.Top() - m_camera->Top();
		if (td < 0.f) m_camera->MovePositionY(td);
		float ld = m_rect.Left() - m_camera->Left();
		if (ld > 0.f) m_camera->MovePositionX(ld);
		float bd = m_rect.Bottom() - m_camera->Bottom();
		if (bd > 0.f) m_camera->MovePositionY(bd);

		m_camera->RoundPosition();
	}

private:
	Camera* m_camera;
	Math::Rectangle m_rect;
};

class PlayerControl : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(PlayerControl, Component);

	PlayerControl() : base(true, false) {}

	void Update() override
	{
		auto actor = EntityAs<Actor>();
		float deltaTime = Engine::Instance()->DeltaTime();
		bool jumpButtonPressed = MInput::GamePads(0)->Pressed(Buttons::A) || MInput::Keyboard()->Pressed(Keys::Space);
		bool jumpButtonCheck = MInput::GamePads(0)->Check(Buttons::A) || MInput::Keyboard()->Check(Keys::Space);
		float extraDropSpeed = -Math_Min(MInput::GamePads(0)->GetLeftStick().y, 0.f) * actor_extra_drop_acceleration;
		if (MInput::Keyboard()->Check(Keys::S)) extraDropSpeed = actor_extra_drop_acceleration;
		if (jumpButtonCheck) {
			extraDropSpeed -= actor_extra_drop_acceleration;
		}
		if (actor->IsOnGround()) {
			m_speedY = 0.f;
			if (jumpButtonPressed) {
				Jump();
			}
		}
		else {
			m_speedY -= (actor_drop_acceleration + extraDropSpeed) * deltaTime;
		}
		if (actor->IsAnythingAbove()) {
			if (m_speedY > 0.f) {
				m_speedY = 0.f;
			}
		}
		float speedX = MInput::GamePads(0)->GetLeftStick(0.2f).x * actor_horizontal_move_speed;
		if (!MInput::GamePads(0)->Attached() || speedX == 0.f) {
			speedX = MInput::Keyboard()->AxisCheck(Keys::A, Keys::D) * actor_horizontal_move_speed;
		}
		actor->Move(Math::Vec2(speedX, m_speedY) * deltaTime);
	}

	void Jump()
	{
		m_speedY = actor_jump_speed;
	}

private:
	float m_speedY = 0.f;
};

class HollowRectGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(HollowRectGraphicsComponent, GraphicsComponent);

	HollowRectGraphicsComponent(int width, int height, float x = 0.f, float y = 0.f)
	: base(false)
	, m_width(width)
	, m_height(height)
	{
		SetColor(Color::Black);
		Position2D(Math::Vec2(x, y));
	}

	void Render() override
	{
		Draw::HollowRect(RenderPosition().x, RenderPosition().y, m_width, m_height, GetColor());
	}

private:
	int m_width;
	int m_height;
};

void MainScene::Begin()
{
	Commands::ExecuteCommand("addslider actor_drop_acceleration 0 3000");
	Commands::ExecuteCommand("addslider actor_extra_drop_acceleration 0 3000");
	Commands::ExecuteCommand("addslider actor_jump_speed 0 1000");
	Commands::ExecuteCommand("addslider actor_horizontal_move_speed 0 1000");

	auto renderer = new TagExcludeRenderer(s_renderTargetTag);
	auto finalRenderer = new SingleTagRenderer(s_renderTargetTag);

	renderer->RenderPass(kNormalRenderPass);
	finalRenderer->RenderPass(kFinalRenderPass);

	auto renderTarget = new RenderTarget2D(360, 252);
	renderer->SetRenderTarget(renderTarget);
	auto screenQuad = new Entity();
	screenQuad->Tag(s_renderTargetTag);
	auto screenImage = new Image(renderTarget->GetTexture2D());
	screenImage->CenterOrigin();
	if (Graphics::IsOriginBottomLeft()) {
		screenImage->SetSpriteEffects(SpriteEffects::FlipVertically);
	}
	screenQuad->Add(screenImage);
	Add(screenQuad);

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 100.f));
	camera->SetSize(Math::Vec2(360.f, 252.f));
	camera->CenterOrigin();
	renderer->KeepCameraCenterOrigin(true);

	auto finalCamera = finalRenderer->GetCamera();
	finalCamera->UseOrthoProjection(true);
	finalCamera->Position(Math::Vec3(0.f, 0.f, 100.f));
	finalCamera->Scale2D(Math::Vec2(2.f));
	finalCamera->CenterOrigin();
	finalRenderer->KeepCameraCenterOrigin(true);

	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	finalRenderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	Add(renderer);
	Add(finalRenderer);

	json::Document map;
	JsonUtil::ReadJsonFile(map, "assets/maps/test.tmj");
	Graphics::UseRenderPass(kNormalRenderPass)->SetClearColor(map["backgroundcolor"].GetString());

	const auto& layerArray = map["layers"].GetArray();
	int tilesX = map["width"].GetInt();
	int tilesY = map["height"].GetInt();
	int tileWidth = map["tilewidth"].GetInt();
	int tileHeight = map["tileheight"].GetInt();
	auto mapEntity = new Entity;
	auto tileSet = new TileSet(GameApp::GetAtlas()->GetAtlasSubtextureAt("tiles_packed", 0), tileWidth, tileHeight);
	for (int layerIndex = 0; layerIndex < layerArray.Size(); ++layerIndex) {
		const auto& layer = layerArray[layerIndex];
		const auto& dataArray = layer["data"].GetArray();
		STL::Vector<STL::Vector<int>> data = STL::Vector<STL::Vector<int>>(tilesX, STL::Vector<int>(tilesY));
		for (int i = 0; i < dataArray.Size(); ++i) {
			int x = i % tilesX;
			int y = i / tilesX;
			y = tilesY - y - 1;
			data[x][y] = dataArray[i].GetInt() - 1;
		}
		if (layerIndex == 0) {
			auto mapGrid = new Grid(tilesX, tilesY, tileWidth, tileHeight);
			for (int x = 0; x < tilesX; ++x) {
				for (int y = 0; y < tilesY; ++y) {
					if (data[x][y] >= 0)
						mapGrid->Set(x, y, true);
				}
			}
			mapEntity->SetCollider(mapGrid);
		}
		auto mapTile = new TileGrid(tileWidth, tileHeight, tilesX, tilesY);
		mapTile->Populate(tileSet, data);
		mapTile->ClipCamera(camera);
		mapEntity->Add(mapTile);
	}

	mapEntity->Tag(s_solidTag);

	Add(mapEntity);

	auto player = new Actor();
	player->Add(new PlayerControl());
	player->Add(new CameraFollow(camera));
	player->Add(new CameraRestrict(camera, 0.f, tilesX * tileWidth, 0.f, tilesY * tileHeight));
	player->Add(new HollowRectGraphicsComponent(10, 16, -5.f, 0.f));
	player->SetCollider(new Hitbox(10.f, 16.f, -5.f, 0.f));
	player->Position2D(Math::Vec2(30.f, 100.f));
	Add(player);

	base::Begin();
}

void MainScene::Update()
{
	// Add scene update here

	base::Update();
}
