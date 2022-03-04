#include "MainScene.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using main::MainScene;

static BitTag s_solidTag("solid");
static BitTag s_climbTag("climb");
static BitTag s_platformTag("platform");
static BitTag s_spikeTag("spike");
static BitTag s_playerTag("player");
static BitTag s_renderTargetTag("render_target");

CHERRYSODA_CONSOLE_VARIABLE(actor_drop_acceleration, float, 1500.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_extra_drop_acceleration, float, 400.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_jump_speed, float, 360.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_horizontal_move_speed, float, 100.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_vertical_climb_speed, float, 100.f, "");

static constexpr int kNormalRenderPass = 1;
static constexpr int kFinalRenderPass = 2;

enum TileType
{
	TILE_EMPTY_BOX = 9,
	TILE_WOW_BOX = 10,
	TILE_COIN_BOX = 11,
	TILE_LOCK = 28,
	TILE_LADDER_TOP = 51,
	TILE_LADDER = 71
};

class Actor : public Entity
{
public:
	typedef Entity base;

	void MoveX(float amount)
	{
		m_remainder.x += amount;
		int move = (int)Math_Round(m_remainder.x);

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
		int move = (int)Math_Round(m_remainder.y);

		if (move != 0) {
			m_remainder.y -= move;
			int sign = Math_Sign(move);

			while (move != 0) {
				bool moveHasNoCollide = !CollideCheck(s_solidTag, Position2D() + Math::Vec2(0, sign));
				if (sign < 0) {
					if (!CollideCheck(s_platformTag, Position2D())) {
						moveHasNoCollide &= !CollideCheck(s_platformTag, Position2D() + Math::Vec2(0, sign));
					}
				}
				if (moveHasNoCollide) {
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
		if (CollideCheck(s_solidTag, Position2D() - Vec2_YUp) || (!CollideCheck(s_platformTag, Position2D()) && CollideCheck(s_platformTag, Position2D() - Vec2_YUp))) {
			m_isOnGround = true;
		}
		m_isAnythingAbove = false;
		if (CollideCheck(s_solidTag, Position2D() + Vec2_YUp)) {
			m_isAnythingAbove = true;
		}
		if (!CollideCheck(s_climbTag)) {
			m_isOnClimb = false;
		}
		else if (MainScene::GetControlAxisY() > 0.5f) {
			m_isOnClimb = true;
		}

		base::Update();
	}

	inline bool IsOnGround() const { return m_isOnGround; }
	inline bool IsAnythingAbove() const { return m_isAnythingAbove; }
	inline bool IsOnClimb() const { return m_isOnClimb; }

private:
	Math::Vec2 m_remainder = Vec2_Zero;
	bool m_isOnGround = false;
	bool m_isAnythingAbove = false;
	bool m_isOnClimb = false;
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
		auto actor = GetEntityAs<Actor>();
		auto spriteSheet = actor->Get<SpriteSheet<StringID>>();
		float deltaTime = Engine::Instance()->DeltaTime();
		bool jumpButtonPressed = MainScene::JumpButtonPressed();
		bool jumpButtonCheck = MainScene::JumpButtonCheck();
		float extraDropSpeed = -Math_Min(MainScene::GetControlAxisY(), 0.f) * actor_extra_drop_acceleration;
		if (jumpButtonCheck) {
			extraDropSpeed -= actor_extra_drop_acceleration;
		}
		if (actor->IsOnClimb() || actor->IsOnGround()) {
			if (jumpButtonPressed) {
				Jump();
			}
			spriteSheet->Play("normal");
		}
		else {
			m_speedY -= (actor_drop_acceleration + extraDropSpeed) * deltaTime;
			spriteSheet->Play("jump");
		}
		if (actor->IsOnClimb()) {
			m_speedY = MainScene::GetControlAxisY() * actor_vertical_climb_speed;
		}
		if (actor->IsOnGround()) {
			if (m_speedY < 0.f) {
				m_speedY = 0.f;
			}
		}
		if (actor->IsAnythingAbove()) {
			if (m_speedY > 0.f) {
				m_speedY = 0.f;
			}
		}
		float speedX = MainScene::GetControlAxisX() * actor_horizontal_move_speed;
		if (speedX > 0.f) {
			spriteSheet->SetSpriteEffects(SpriteEffects::FlipHorizontally);
		}
		else if (speedX < 0.f) {
			spriteSheet->SetSpriteEffects(SpriteEffects::None);
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

void MainScene::Begin()
{
	Commands::ExecuteCommand("addslider actor_drop_acceleration 0 3000");
	Commands::ExecuteCommand("addslider actor_extra_drop_acceleration 0 3000");
	Commands::ExecuteCommand("addslider actor_jump_speed 0 1000");
	Commands::ExecuteCommand("addslider actor_horizontal_move_speed 0 1000");

	// Initialize Renderers
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

	// Load Map
	json::Document map;
	JsonUtil::ReadJsonFile(map, "assets/maps/test.tmj");
	Graphics::UseRenderPass(kNormalRenderPass)->SetClearColor(map["backgroundcolor"].GetString());

	const auto& layerArray = map["layers"].GetArray();
	int tilesX = map["width"].GetInt();
	int tilesY = map["height"].GetInt();
	int tileWidth = map["tilewidth"].GetInt();
	int tileHeight = map["tileheight"].GetInt();
	int mapHeight = tilesY * tileHeight;
	auto mapEntity = new Entity();
	mapEntity->AutoDeleteWhenRemoved();
	mapEntity->Tag(s_solidTag);
	Add(mapEntity);
	auto tileSet = new TileSet(GameApp::GetAtlas()->Get("tiles_packed"), tileWidth, tileHeight);
	for (int layerIndex = 0; layerIndex < static_cast<int>(layerArray.Size()); ++layerIndex) {
		const auto& layer = layerArray[layerIndex];
		if (layer["data"].IsArray()) {
			const auto& dataArray = layer["data"].GetArray();
			STL::Vector<STL::Vector<int>> data = STL::Vector<STL::Vector<int>>(tilesX, STL::Vector<int>(tilesY));
			for (int i = 0; i < static_cast<int>(dataArray.Size()); ++i) {
				int x = i % tilesX;
				int y = i / tilesX;
				y = tilesY - y - 1;
				data[x][y] = dataArray[i].GetInt() - 1;
			}
			if (layer["name"].GetString() == String("collision")) {
				auto mapGrid = new Grid(tilesX * 2, tilesY * 2, tileWidth / 2, tileHeight / 2);
				mapGrid->AutoDeleteWhenRemoved();
				for (int x = 0; x < tilesX; ++x) {
					for (int y = 0; y < tilesY; ++y) {
						int v = data[x][y];
						if (v >= 0) {
							// TODO: Make sub-tile collision data driven
							int xx = x * 2;
							int yy = y * 2;
							if (v != 98 && v != 118) {
								if (v != 99 && v != 119) {
									mapGrid->Set(xx, yy, true);
									mapGrid->Set(xx + 1, yy, true);
								}
								mapGrid->Set(xx, yy + 1, true);
							}
							mapGrid->Set(xx + 1, yy + 1, true);
						}
					}
				}
				mapEntity->SetCollider(mapGrid);
			}
			else if (layer["name"].GetString() == String("spike")) {
				auto spikeEntity = new Entity();
				spikeEntity->AutoDeleteWhenRemoved();
				auto spikeGrid = new Grid(tilesX, tilesY * 2, tileWidth, tileHeight / 2);
				spikeGrid->AutoDeleteWhenRemoved();
				for (int x = 0; x < tilesX; ++x) {
					for (int y = 0; y < tilesY; ++y) {
						int v = data[x][y];
						if (v >= 0) {
							spikeGrid->Set(x, y * 2, true);
						}
					}
				}
				spikeEntity->SetCollider(spikeGrid);
				spikeEntity->Tag(s_spikeTag);
				Add(spikeEntity);
			}
			auto mapTile = new TileGrid(tileWidth, tileHeight, tilesX, tilesY);
			mapTile->AutoDeleteWhenRemoved();
			mapTile->Populate(tileSet, data);
			mapTile->ClipCamera(camera);
			mapEntity->Add(mapTile);
		}
		else if (layer["objects"].IsArray()) {
			const auto& objectArray = layer["objects"].GetArray();
			for (const auto& object : objectArray) {
				int id = object["gid"].GetInt() - 1;
				int x = object["x"].GetInt();
				int y = mapHeight - object["y"].GetInt();
				auto entity = new Entity(Math::Vec2(x, y));
				entity->AutoDeleteWhenRemoved();
				Add(entity);
				InitializeTileObject(id, entity, tileWidth, tileHeight);
			}
		}
	}

	// Initialize Player
	auto player = new Actor();
	player->Add(new PlayerControl());
	player->Add(new CameraFollow(camera));
	player->Add(new CameraRestrict(camera, 0.f, tilesX * tileWidth, 0.f, tilesY * tileHeight));
	auto spriteSheet = new SpriteSheet<StringID>(GameApp::GetAtlas()->Get("characters_packed"), 24, 24);
	spriteSheet->Add("normal", 0);
	spriteSheet->Add("jump", 1);
	spriteSheet->Play("normal");
	spriteSheet->JustifyOrigin(Math::Vec2(0.5f, 0.f));
	player->Add(spriteSheet);
	player->SetCollider(new Hitbox(12.f, 16.f, -6.f, 0.f));
	player->Position2D(Math::Vec2(30.f, 100.f));
	player->Tag(s_playerTag);
	Add(player);

	base::Begin();
}

void MainScene::InitializeTileObject(int id, Entity* entity, int tileWidth, int tileHeight)
{
	auto spriteSheet = new SpriteSheet<int>(GameApp::GetAtlas()->Get("tiles_packed"), tileWidth, tileHeight);
	spriteSheet->AutoDeleteWhenRemoved();
	entity->Add(spriteSheet);

	spriteSheet->Add(0, id);
	spriteSheet->Play(0);
	auto hitbox = new Hitbox(tileWidth, tileHeight);
	hitbox->AutoDeleteWhenRemoved();
	entity->SetCollider(hitbox);

	switch (id) {
	case TILE_LADDER_TOP:
		entity->AddTag(s_platformTag);
		hitbox->Height(tileHeight - 3);
	case TILE_LADDER:
		entity->AddTag(s_climbTag);
		break;
	case TILE_LOCK:
	case TILE_EMPTY_BOX:
	case TILE_WOW_BOX:
	case TILE_COIN_BOX:
		entity->AddTag(s_solidTag);
		break;
	}
}

float MainScene::GetControlAxisX()
{
	float axisX = MInput::GamePads(0)->GetLeftStick(0.2f).x;
	if (!MInput::GamePads(0)->Attached() || axisX == 0.f) {
		axisX = MInput::Keyboard()->AxisCheck(Keys::A, Keys::D);
		if (axisX == 0.f) {
			axisX = MInput::Keyboard()->AxisCheck(Keys::Left, Keys::Right);
		}
	}
	return axisX;
}

float MainScene::GetControlAxisY()
{
	float axisY = MInput::GamePads(0)->GetLeftStick(0.2f).y;
	if (!MInput::GamePads(0)->Attached() || axisY == 0.f) {
		axisY = MInput::Keyboard()->AxisCheck(Keys::S, Keys::W);
		if (axisY == 0.f) {
			axisY = MInput::Keyboard()->AxisCheck(Keys::Down, Keys::Up);
		}
	}
	return axisY;
}

bool MainScene::JumpButtonPressed()
{
	return MInput::GamePads(0)->Pressed(Buttons::A) || MInput::Keyboard()->Pressed(Keys::Space, Keys::J);
}

bool MainScene::JumpButtonCheck()
{
	return MInput::GamePads(0)->Check(Buttons::A) || MInput::Keyboard()->Check(Keys::Space, Keys::J);
}
