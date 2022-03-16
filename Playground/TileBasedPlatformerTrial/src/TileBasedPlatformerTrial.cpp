#include "TileBasedPlatformerTrial.h"

#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using tilebasedplatformertrial::TileBasedPlatformerTrial;

using main::MainScene;

using namespace cherrysoda;

Atlas* TileBasedPlatformerTrial::ms_atlas = nullptr;

TileBasedPlatformerTrial::TileBasedPlatformerTrial()
	: base(720, 504, "TileBasedPlatformerTrial (WIP)")
{
	SetClearColor(Color::Black);
}

void TileBasedPlatformerTrial::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void TileBasedPlatformerTrial::Initialize()
{
	base::Initialize();

	Graphics::SetPointTextureSampling();

	auto scene = new MainScene();

	SetScene(scene);
}

void TileBasedPlatformerTrial::LoadContent()
{
	base::LoadContent();

	ms_atlas = Atlas::FromAtlas("assets/atlases/atlas.json");
	Audio::LoadFile("coin", "assets/sounds/coin.wav");
	Audio::LoadFile("jump", "assets/sounds/jump.wav");
}

void TileBasedPlatformerTrial::UnloadContent()
{
	delete ms_atlas;
	ms_atlas = nullptr;

	base::UnloadContent();
}
