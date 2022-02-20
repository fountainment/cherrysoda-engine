#include "MainScene.h"

#include "Program.h"

#include <CherrySoda/CherrySoda.h>

using namespace cherrysoda;

using main::MainScene;

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

	Add(mapEntity);

	base::Begin();
}

void MainScene::Update()
{
	// Add scene update here

	base::Update();
}
