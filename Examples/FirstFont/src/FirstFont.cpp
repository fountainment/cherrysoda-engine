#include "FirstFont.h"

#include <CherrySoda/CherrySoda.h>

using firstfont::FirstFont;

using namespace cherrysoda;

FirstFont::FirstFont()
	: base()
{
	SetTitle("FirstFont");
	SetClearColor(Color::Black);
}

void FirstFont::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void FirstFont::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new Scene();

	auto entity = new Entity();
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->UseOrthoProjection(true);
	camera->Position(Math::Vec3(0.f, 0.f, 200.f));
	camera->CenterOrigin();

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	scene->Add(entity);
	scene->Add(renderer);

	auto font = new PixelFont("test");
	auto atlas = Atlas::FromAtlas("assets/atlases/atlas.json");
	font->AddFontSize("assets/fonts/font.json", atlas);

	auto pixelText = new PixelText(font, u8"你好, 世界!\nHello, World!", Color::White);
	pixelText->Position(Math::Vec2(-100.f, 30.f));
	entity->Add(pixelText);

	SetScene(scene);
}

void FirstFont::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}

