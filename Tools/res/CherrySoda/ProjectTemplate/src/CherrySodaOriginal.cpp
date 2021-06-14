#include "CherrySodaOriginal.h"

#include <CherrySoda/CherrySoda.h>

using cherrysodatemplate::CherrySodaTemplate;

using namespace cherrysoda;

CherrySodaTemplate::CherrySodaTemplate()
	: base()
{
	SetTitle("CherrySodaOriginal");
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
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 100.f));
	camera->CenterOrigin();

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));
	scene->Add(renderer);

	SetScene(scene);
}

void CherrySodaTemplate::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

