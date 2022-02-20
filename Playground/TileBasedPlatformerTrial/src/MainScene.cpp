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
	auto mapTile = new TileGrid(18, 18, 40, 23);

	STL::Vector<STL::Vector<int>> data(40, STL::Vector<int>(23, 0));
	mapTile->Populate(tileSet, data);
	mapTile->ClipCamera(camera);

	mapGrid->Set(1, 1, true);

	mapEntity->Add(mapTile);
	mapEntity->SetCollider(mapGrid);

	Add(mapEntity);

	base::Begin();
}

void MainScene::Update()
{
	// Add scene update here

	base::Update();
}
