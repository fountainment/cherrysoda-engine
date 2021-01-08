#include "sfxr_port.h"

#include "sfxr/sfxr.h"

#include <CherrySoda/CherrySoda.h>

using sfxr_port::Sfxr_port;

using namespace cherrysoda;

Sfxr_port::Sfxr_port()
	: base()
{
	SetTitle("sfxr_port");
	SetClearColor(Color::Black);
}

void Sfxr_port::Update()
{
	base::Update();

	SfxrUpdate();
	// Add global GUI or other global stuffs here
}

void Sfxr_port::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	SfxrInit();
	auto scene = new Scene();
	SetScene(scene);
}

void Sfxr_port::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

