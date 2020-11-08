#include "Noise.h"

#include <CherrySoda/CherrySoda.h>

using noise::Noise;

using namespace cherrysoda;

Noise::Noise()
	: base()
{
	SetTitle("Noise");
	SetClearColor(Color::Black);
}

void Noise::Update()
{
	base::Update();
}

void Noise::Initialize()
{
	base::Initialize();

	unsigned char data[200 * 200 * 4] = {0xFF};
	std::memset(data, 0xFF, 200 * 200 * 4);
	for (int i = 0; i < 200 * 200; ++i) {
		data[i * 4    ] = Calc::GetRandom().Next(256);
		data[i * 4 + 1] = Calc::GetRandom().Next(256);
		data[i * 4 + 2] = Calc::GetRandom().Next(256);
	}

	auto texture = Texture2D::FromRGBA(data, 200, 200);
	auto image = new Image(texture);
	auto entity = new Entity();
	auto scene = new Scene();
	auto renderer = new EverythingRenderer();

	renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 200.f));
	renderer->SetEffect(Graphics::GetEmbeddedEffect("sprite"));

	image->CenterOrigin();
	entity->Add(image);
	scene->Add(entity);
	scene->Add(renderer);

	SetScene(scene);
}

void Noise::LoadContent()
{
	base::LoadContent();
}

