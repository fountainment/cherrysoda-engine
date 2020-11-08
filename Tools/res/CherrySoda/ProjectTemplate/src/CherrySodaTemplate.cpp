#include "CherrySodaTemplate.h"

#include <CherrySoda/CherrySoda.h>

using cherrysodatemplate::CherrySodaTemplate;

using namespace cherrysoda;

CherrySodaTemplate::CherrySodaTemplate()
	: base()
{
	SetTitle("CherrySodaTemplate");
	SetClearColor(Color::Black);
}

void CherrySodaTemplate::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void CherrySodaTemplate::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new Scene();
	SetScene(scene);
}

void CherrySodaTemplate::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

