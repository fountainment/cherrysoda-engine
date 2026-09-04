#include "Noise.h"

#include <CherrySoda/CherrySoda.h>

#include <cstddef>

using noise::Noise;

using namespace cherrysoda;

static STL::Action<> s_updateAction;

Noise::Noise()
{
	SetTitle("Noise");
	SetClearColor(Color::Black);
}

void Noise::Update()
{
	base::Update();

	if (s_updateAction) {
		s_updateAction();
	}
}

void Noise::Initialize()
{
	base::Initialize();

	auto* data = new unsigned char[static_cast<size_t>(200 * 200 * 4)];
	std::memset(data, 0xFF, static_cast<size_t>(200 * 200 * 4));
	for (int i = 0; i < 200 * 200; ++i) {
		data[static_cast<ptrdiff_t>(i * 4)] = Calc::GetRandom()->Next(256);
		data[(i * 4) + 1] = Calc::GetRandom()->Next(256);
		data[(i * 4) + 2] = Calc::GetRandom()->Next(256);
	}
	auto texture = Texture2D::FromRGBA(data, 200, 200);
	delete[] data;

	auto* image = new Image(texture);
	auto* entity = new Entity();
	auto* scene = new Scene();
	auto* renderer = new EverythingRenderer();

	s_updateAction = [image] { image->RotateOnZ(Engine::Instance()->DeltaTime()); };

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
