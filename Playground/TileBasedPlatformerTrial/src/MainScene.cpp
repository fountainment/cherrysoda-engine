#include "MainScene.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using main::MainScene;

static BitTag s_solidTag("solid");

class Actor : public Entity
{
public:
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

		Entity::Update();
	}

	inline bool IsOnGround() const { return m_isOnGround; }
	inline bool IsAnythingAbove() const { return m_isAnythingAbove; }

private:
	Math::Vec2 m_remainder;
	bool m_isOnGround = false;
	bool m_isAnythingAbove = false;
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
		float extraDropSpeed = -Math_Min(MInput::GamePads(0)->GetLeftStick().y, 0.f) * 300.f;
		if (MInput::Keyboard()->Check(Keys::S)) extraDropSpeed = 300.f;
		if (jumpButtonCheck) {
			extraDropSpeed -= 400.f;
		}
		if (actor->IsOnGround()) {
			m_speedY = 0.f;
			if (jumpButtonPressed) {
				Jump();
			}
		}
		else {
			m_speedY -= (1000.f + extraDropSpeed) * deltaTime;
		}
		if (actor->IsAnythingAbove()) {
			if (m_speedY > 0.f) {
				m_speedY = 0.f;
			}
		}
		float speedX = MInput::GamePads(0)->GetLeftStick(0.2f).x * 100.f;
		if (!MInput::GamePads(0)->Attached() || speedX == 0.f) {
			speedX = MInput::Keyboard()->AxisCheck(Keys::A, Keys::D) * 100.f;
		}
		static_cast<Actor*>(GetEntity())->Move(Math::Vec2(speedX, m_speedY) * deltaTime);
	}

	void Jump()
	{
		m_speedY = 260.f;
	}

private:
	float m_speedY = 0.f;

};

class HollowRectGraphicsComponent : public GraphicsComponent
{
public:
	CHERRYSODA_DECLARE_COMPONENT(HollowRectGraphicsComponent, GraphicsComponent);

	HollowRectGraphicsComponent(int width, int height)
	: base(false)
	, m_width(width)
	, m_height(height)
	{
		SetColor(Color::Black);
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
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 100.f));
	camera->Scale2D(Math::Vec2(2.f));

	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	Add(renderer);

	auto mapEntity = new Entity;
	auto tileSet = new TileSet(GameApp::GetAtlas()->GetAtlasSubtextureAt("tiles_packed", 0), 18, 18);
	auto mapGrid = new Grid(40, 23, 18, 18);

	json::Document json;
	JsonUtil::ReadJsonFile(json, "assets/maps/test.tmj");
	Engine::Instance()->SetClearColor(json["backgroundcolor"].GetString());
	
	const auto& layerArray = json["layers"].GetArray();
	for (int n = 0; n < layerArray.Size(); ++n) {
		int i = 0;
		STL::Vector<STL::Vector<int>> data = STL::Vector<STL::Vector<int>>(40, STL::Vector<int>(23));
		for (auto& value : layerArray[n]["data"].GetArray()) {
			int v = value.GetInt();
			--v;
			int x = i % 40;
			int y = i / 40;
			y = 22 - y;
			data[x][y] = v;
			++i;
		}
		if (n == 0) {
			for (int x = 0; x < 40; ++x) {
				for (int y = 0; y < 23; ++y) {
					if (data[x][y] >= 0) mapGrid->Set(x, y, true);
				}
			}
		}
		auto mapTile = new TileGrid(18, 18, 40, 23);
		mapTile->Populate(tileSet, data);
		mapTile->ClipCamera(camera);
		mapEntity->Add(mapTile);
	}

	mapEntity->SetCollider(mapGrid);
	mapEntity->Tag(s_solidTag);

	Add(mapEntity);

	auto player = new Actor();
	player->Add(new PlayerControl());
	player->Add(new HollowRectGraphicsComponent(10, 16));
	player->SetCollider(new Hitbox(10, 16));
	player->PositionY(200.f);
	Add(player);

	base::Begin();
}

void MainScene::Update()
{
	// Add scene update here

	base::Update();
}
