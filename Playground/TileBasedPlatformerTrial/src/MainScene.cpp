#include "MainScene.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using main::MainScene;

static BitTag s_solidTag("solid");
static BitTag s_climbTag("climb");
static BitTag s_pickUpItemTag("pickupitem");
static BitTag s_platformTag("platform");
static BitTag s_playerTag("player");
static BitTag s_renderTargetTag("render_target");
static BitTag s_spikeTag("spike");
static BitTag s_lockTag("lock");
static BitTag s_boxTag("box");

CHERRYSODA_CONSOLE_VARIABLE(actor_drop_acceleration, float, 1500.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_extra_drop_acceleration, float, 400.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_jump_speed, float, 360.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_horizontal_move_speed, float, 100.f, "");
CHERRYSODA_CONSOLE_VARIABLE(actor_vertical_climb_speed, float, 100.f, "");
CHERRYSODA_CONSOLE_VARIABLE(camera_follow_rect_width, float, 100.f, "");
CHERRYSODA_CONSOLE_VARIABLE(camera_follow_rect_height, float, 60.f, "");

static constexpr int kNormalRenderPass = 1;
static constexpr int kFinalRenderPass = 2;

enum TileType
{
	TILE_EMPTY_BOX = 9,
	TILE_WOW_BOX = 10,
	TILE_COIN_BOX = 11,
	TILE_KEY = 27,
	TILE_LOCK = 28,
	TILE_LADDER_TOP = 51,
	TILE_DIAMOND = 67,
	TILE_LADDER = 71,
	TILE_ROPE_H_LEFT = 90,
	TILE_ROPE_H_MIDDLE = 91,
	TILE_ROPE_H_RIGHT = 92,
	TILE_PLATFORM_0 = 146,
	TILE_PLATFORM_1 = 147,
	TILE_COIN = 151
};


class Actor;


class PickUpItemCallbackComponent : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(PickUpItemCallbackComponent, Component);

	PickUpItemCallbackComponent(STL::Action<Actor*> onPickUp) : base(false, false) { OnPickUp(onPickUp); }

	void OnPickUp(STL::Action<Actor*> onPickUp) { m_onPickUp = onPickUp; }
	void CallOnPickUp(Actor* actor) { if (m_onPickUp != nullptr) m_onPickUp(actor); }

private:
	STL::Action<Actor*> m_onPickUp = nullptr;
};


class InventoryComponent : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(InventoryComponent, Component);

	InventoryComponent() : base(false, false) {}

	void AddItem(const StringID& id, int amount = 1)
	{
		++m_items[id];
	}

	bool RemoveItem(const StringID& id, int amount = 1)
	{
		if (STL::ContainsKey(m_items, id)) {
			if (m_items[id] > 0) {
				--m_items[id];
				return true;
			}
		}
		return false;
	}

	int Get(const StringID& id)
	{
		if (!STL::ContainsKey(m_items, id)) {
			return 0;
		}
		return m_items[id];
	}

private:
	STL::HashMap<StringID, int> m_items;
};


class FrontSensorComponent : public CollidableComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(FrontSensorComponent, CollidableComponent);

	FrontSensorComponent() : base(false, false, true) {}
};


class UpSensorComponent : public CollidableComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(UpSensorComponent, CollidableComponent);

	UpSensorComponent() : base(false, false, true) {}
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
				bool moveHasNoCollide = !CollideCheck(s_solidTag, Position2D() + Math::Vec2(sign, 0));
				if (!CollideCheck(s_platformTag)) {
					moveHasNoCollide &= !CollideCheck(s_platformTag, Position2D() + Math::Vec2(sign, 0));
				}
				if (moveHasNoCollide) {
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
					if (!CollideCheck(s_platformTag)) {
						moveHasNoCollide &= !CollideCheck(s_platformTag, Position2D() + Math::Vec2(0, sign));
					}
				}
				if (!moveHasNoCollide && sign > 0) {
					for (auto box : GetScene()->Get(s_boxTag)) {
						if (box->CollideCheck(Get<UpSensorComponent>())) {
							box->Get<SpriteSheet<int>>()->Play(1);
						}
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
		if (CollideCheck(s_solidTag, Position2D() - Vec2_YUp) || (!CollideCheck(s_platformTag) && CollideCheck(s_platformTag, Position2D() - Vec2_YUp))) {
			m_isOnGround = true;
		}
		m_isAnythingAbove = false;
		if (CollideCheck(s_solidTag, Position2D() + Vec2_YUp)) {
			m_isAnythingAbove = true;
		}
		if (m_isOnClimb && !CollideCheck(s_climbTag)) {
			m_isOnClimb = false;
		}

		if (CollideCheck(s_spikeTag)) {
			Position2D(Math::Vec2(30.f, 100.f));
		}

		auto pickUpItem = CollideFirst(s_pickUpItemTag);
		if (pickUpItem != nullptr) {
			auto pickUpItemCallbackComp = pickUpItem->Get<PickUpItemCallbackComponent>();
			if (pickUpItemCallbackComp) {
				pickUpItemCallbackComp->CallOnPickUp(this);
			}
			pickUpItem->RemoveSelf();
		}

		auto frontSensor = Get<FrontSensorComponent>();
		auto invectory = Get<InventoryComponent>();
		if (frontSensor && invectory) {
			for (auto lock : GetScene()->Get(s_lockTag)) {
				if (lock->CollideCheck(frontSensor)) {
					if (invectory->RemoveItem("key")) {
						lock->RemoveSelf();
					}
				}
			}
		}

		base::Update();
	}

	void TryClimb() { if (CollideCheck(s_climbTag)) { m_isOnClimb = true; } }
	void CancelClimb() { m_isOnClimb = false; }

	void GetDownPlatform()
	{
		if (!CollideCheck(s_solidTag, Position2D() - Vec2_YUp) && !CollideCheck(s_platformTag) && CollideCheck(s_platformTag, Position2D() - Vec2_YUp)) {
			MovePositionY(-1.f);
			TryClimb();
		}
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

	CameraFollow(Camera* camera, const Math::Rectangle& followRect, const Math::Rectangle& limitRect)
		: base(true, false)
		, m_camera(camera)
		, m_followRect(followRect)
		, m_limitRect(limitRect)
	{}

	void Update() override
	{
		m_followRect =
			Math::Rectangle{
				Math::Vec2(camera_follow_rect_width, camera_follow_rect_height) * -0.5f,
				Math::Vec2(camera_follow_rect_width, camera_follow_rect_height)
			};

		auto followRectCopy = m_followRect;
		auto limitRectCopy = m_limitRect;
		followRectCopy.Move(GetEntity()->Position2D());
		auto shrinkBuffer = Math::Vec2(10.f);
		auto shrinkSize = m_camera->Size() / m_camera->Scale2D() * 0.5f - shrinkBuffer;
		limitRectCopy.Move(shrinkSize);
		limitRectCopy.Size(limitRectCopy.Size() - shrinkSize * 2.f);

		auto target = m_camera->Position2D();
		target = followRectCopy.Clamp(target);
		target = limitRectCopy.Clamp(target);

		m_camera->Approach(target, 0.1f);
		m_camera->RoundPosition();

		shrinkSize = m_camera->Size() / m_camera->Scale2D() * 0.5f;
		limitRectCopy = m_limitRect;
		limitRectCopy.Move(shrinkSize);
		limitRectCopy.Size(limitRectCopy.Size() - shrinkSize * 2.f);
		m_camera->Position2D(limitRectCopy.Clamp(m_camera->Position2D()));
		m_camera->RoundPosition();
	}

	void DebugRender(Camera* camera) override
	{
		auto followRectCopy = m_followRect;
		followRectCopy.Move(camera->Position2D());
		Draw::HollowRect(followRectCopy);
		Draw::HollowRect(m_limitRect, Color::Red);
	}

private:
	Camera* m_camera = nullptr;
	Math::Rectangle m_followRect;
	Math::Rectangle m_limitRect;
};


class CameraAutoScale : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(CameraAutoScale, Component);

	CameraAutoScale(Camera* camera, Camera* scaleCamera)
		: base(true, false)
		, m_camera(camera)
		, m_scaleCamera(scaleCamera)
	{}

	void Update() override
	{
		Math::IVec2 winSize = Engine::Instance()->GetWindowSize();
		Math::IVec2 cameraSize = m_camera->GetSize();
		Math::IVec2 scaleVec = winSize / cameraSize;
		int scale = Math_Max(1, Math_Min(scaleVec.x, scaleVec.y));
		m_scaleCamera->Scale2D(Math::Vec2(scale));
	}

private:
	Camera* m_camera;
	Camera* m_scaleCamera;
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
		if (MainScene::GetControlAxisY() > 0.5f) {
			actor->TryClimb();
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
		if (MainScene::GetControlDownPressed()) {
			actor->GetDownPlatform();
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
			actor->Get<FrontSensorComponent>()->GetCollider()->PositionX(0.f);
		}
		else if (speedX < 0.f) {
			spriteSheet->SetSpriteEffects(SpriteEffects::None);
			actor->Get<FrontSensorComponent>()->GetCollider()->PositionX(-8.f);
		}
		actor->Move(Math::Vec2(speedX, m_speedY) * deltaTime);
	}

	void Jump()
	{
		auto actor = GetEntityAs<Actor>();
		m_speedY = actor_jump_speed;
		actor->CancelClimb();
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
	Commands::ExecuteCommand("addslider actor_vertical_climb_speed 0 1000");
	Commands::ExecuteCommand("addslider camera_follow_rect_width 0 500");
	Commands::ExecuteCommand("addslider camera_follow_rect_height 0 500");
	Commands::ExecuteCommand("clear");

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
		if (layer.HasMember("data") && layer["data"].IsArray()) {
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
		else if (layer.HasMember("objects") && layer["objects"].IsArray()) {
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
	player->Add(new CameraAutoScale(camera, finalCamera));
	player->Add(new CameraFollow(camera,
		Math::Rectangle{ Math::Vec2(camera_follow_rect_width, camera_follow_rect_height) * -0.5f, Math::Vec2(camera_follow_rect_width, camera_follow_rect_height) },
		Math::Rectangle{ Vec2_Zero, Math::Vec2(tilesX * tileWidth, tilesY * tileHeight) }));
	player->Add(new InventoryComponent);
	auto spriteSheet = new SpriteSheet<StringID>(GameApp::GetAtlas()->Get("characters_packed"), 24, 24);
	spriteSheet->Add("normal", 0);
	spriteSheet->Add("jump", 1);
	spriteSheet->Play("normal");
	spriteSheet->JustifyOrigin(Math::Vec2(0.5f, 0.f));
	player->Add(spriteSheet);
	player->SetCollider(new Hitbox(12.f, 16.f, -6.f, 0.f));
	player->Position2D(Math::Vec2(30.f, 100.f));
	player->Tag(s_playerTag);
	auto frontSensorHitbox = new Hitbox(8.f, 12.f, -8.f, 2.f);
	frontSensorHitbox->AutoDeleteWhenRemoved();
	auto upSensorHitbox = new Hitbox(12.f, 3.f, -6.f, 14.f);
	upSensorHitbox->AutoDeleteWhenRemoved();
	auto frontSensorComponent = new FrontSensorComponent();
	frontSensorComponent->SetCollider(frontSensorHitbox);
	auto upSensorComponent = new UpSensorComponent();
	upSensorComponent->SetCollider(upSensorHitbox);
	player->Add(frontSensorComponent);
	player->Add(upSensorComponent);
	player->AutoDeleteAllInsideWhenRemoved();
	Add(player);

	base::Begin();
}


void MainScene::InitializeTileObject(int id, Entity* entity, int tileWidth, int tileHeight)
{
	auto spriteSheet = new SpriteSheet<int>(GameApp::GetAtlas()->Get("tiles_packed"), tileWidth, tileHeight);
	entity->Add(spriteSheet);

	spriteSheet->Add(0, id);
	spriteSheet->Play(0);
	auto hitbox = new Hitbox(tileWidth, tileHeight);
	entity->SetCollider(hitbox);

	switch (id) {
	case TILE_LADDER_TOP:
		entity->AddTag(s_platformTag);
		hitbox->Height(15.f);
	case TILE_LADDER:
		entity->AddTag(s_climbTag);
		break;
	case TILE_LOCK:
		entity->AddTag(s_lockTag);
		entity->AddTag(s_solidTag);
		break;
	case TILE_EMPTY_BOX:
	case TILE_WOW_BOX:
	case TILE_COIN_BOX:
		entity->AddTag(s_solidTag);
		entity->AddTag(s_boxTag);
		spriteSheet->Add(1, TILE_EMPTY_BOX);
		break;
	case TILE_PLATFORM_0:
		entity->AddTag(s_climbTag);
	case TILE_PLATFORM_1:
		entity->AddTag(s_platformTag);
		hitbox->PositionY(7.f);
		hitbox->Height(11.f);
		break;
	case TILE_KEY:
		hitbox->PositionY(3.f);
		hitbox->Height(12.f);
		entity->AddTag(s_pickUpItemTag);
		entity->Add(new PickUpItemCallbackComponent([](Entity* entity){ entity->Get<InventoryComponent>()->AddItem("key"); }));
		break;
	case TILE_DIAMOND:
		hitbox->Position2D(Math::Vec2(2.f));
		hitbox->Width(14.f);
		hitbox->Height(14.f);
		entity->AddTag(s_pickUpItemTag);
		entity->Add(new PickUpItemCallbackComponent([](Entity* entity){ entity->Get<InventoryComponent>()->AddItem("diamond"); }));
		break;
	case TILE_ROPE_H_LEFT:
	case TILE_ROPE_H_MIDDLE:
	case TILE_ROPE_H_RIGHT:
		entity->AddTag(s_platformTag);
		hitbox->Height(6.f);
		hitbox->PositionY(6.f);
		break;
	case TILE_COIN:
		entity->AddTag(s_pickUpItemTag);
		entity->Add(new PickUpItemCallbackComponent([](Entity* entity){ entity->Get<InventoryComponent>()->AddItem("coin"); }));
		spriteSheet->Add(1, 0.5f, { TILE_COIN, TILE_COIN + 1 });
		spriteSheet->Play(1);
		break;
	}

	entity->AutoDeleteAllInsideWhenRemoved();
}


float MainScene::GetControlAxisX()
{
	float axisX = MInput::GamePads(0)->LeftStickHorizontal(0.2f);
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
	float axisY = MInput::GamePads(0)->LeftStickVertical(0.2f);
	if (!MInput::GamePads(0)->Attached() || axisY == 0.f) {
		axisY = MInput::Keyboard()->AxisCheck(Keys::S, Keys::W);
		if (axisY == 0.f) {
			axisY = MInput::Keyboard()->AxisCheck(Keys::Down, Keys::Up);
		}
	}
	return axisY;
}


bool MainScene::GetControlDownPressed()
{
	bool result = MInput::GamePads(0)->LeftStickDownPressed(0.2f);
	if (!MInput::GamePads(0)->Attached() || !result) {
		result = MInput::Keyboard()->Pressed(Keys::S, Keys::Down);
	}
	return result;
}


bool MainScene::JumpButtonPressed()
{
	return MInput::GamePads(0)->Pressed(Buttons::A) || MInput::Keyboard()->Pressed(Keys::Space, Keys::J);
}


bool MainScene::JumpButtonCheck()
{
	return MInput::GamePads(0)->Check(Buttons::A) || MInput::Keyboard()->Check(Keys::Space, Keys::J);
}
