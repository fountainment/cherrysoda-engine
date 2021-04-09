#include "PlatformerTrial.h"

#include "MainScene.h"

#include <CherrySoda/CherrySoda.h>

using platformertrial::PlatformerTrial;

using namespace cherrysoda;

PlatformerTrial::PlatformerTrial()
	: base(960, 540, 960, 540, "PlatformerTrial", false)
{
	SetClearColor(Color::Black);
}

void PlatformerTrial::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void PlatformerTrial::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new main::MainScene();
	SetScene(scene);
}

void PlatformerTrial::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}
