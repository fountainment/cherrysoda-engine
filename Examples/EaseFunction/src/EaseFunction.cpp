#include "EaseFunction.h"

#include <CherrySoda/CherrySoda.h>

using easefunction::EaseFunction;

using namespace cherrysoda;

static STL::Vector<STL::Func<Math::Vec3,float>> s_posFunctions;
static MeshGraphicsComponent<Graphics::PosColorVertex>* s_points;

EaseFunction::EaseFunction()
	: base()
{
	SetTitle("EaseFunction");
	SetClearColor(Color::Black);
}

void EaseFunction::Update()
{
	base::Update();

	static float time = 0.f;
	float duration = 1.5f;
	float gap = 1.f;
	time += Engine::Instance()->DeltaTime();
	if (time > (duration + gap)) {
		time = 0.f;
	}

	s_points->GetMesh()->Clear();
	if (time <= duration) {
		float x = time / duration;
		Math::Vec3 size(1.f, 1.f, 0.f);
		size *= 0.08f;
		for (auto posFunc : s_posFunctions) {
			auto origin = posFunc(x) - size * 0.5f;
			s_points->GetMesh()->AddQuad(
				Graphics::PosColorVertex::MakeVertex(origin + size),
				Graphics::PosColorVertex::MakeVertex(origin + Math::Vec3(0.f, size.y, 0.f)),
				Graphics::PosColorVertex::MakeVertex(origin + Math::Vec3(size.x, 0.f, 0.f)),
				Graphics::PosColorVertex::MakeVertex(origin)
			);
		}
	}
	s_points->GetMesh()->SubmitBuffer();
}

void EaseFunction::Initialize()
{
	base::Initialize();

	auto scene = new Scene();
	auto entity = new Entity();
	auto lines = new MeshGraphicsComponent<Graphics::PosColorVertex>(true);
	s_points = new MeshGraphicsComponent<Graphics::PosColorVertex>(true);
	auto renderer = new EverythingRenderer();
	renderer->GetCamera()->Position(Math::Vec3(7.5f, -2.f, 15.f));

	lines->GetMesh()->SetPrimitiveType(Graphics::PrimitiveType::Lines);
	int pointNum = 300;
	STL::Vector<Easer> easers = { Ease::SineIn, Ease::SineOut, Ease::SineInOut,
	                              Ease::QuadIn, Ease::QuadOut, Ease::QuadInOut,
	                              Ease::CubeIn, Ease::CubeOut, Ease::CubeInOut,
	                              Ease::QuintIn, Ease::QuintOut, Ease::QuintInOut,
	                              Ease::ExpoIn, Ease::ExpoOut, Ease::ExpoInOut,
	                              Ease::BackIn, Ease::BackOut, Ease::BackInOut,
	                              Ease::ElasticIn, Ease::ElasticOut, Ease::ElasticInOut,
	                              Ease::BounceIn, Ease::BounceOut, Ease::BounceInOut};
	for (int k = 0; k < (int)easers.size(); ++k) {
		Math::Vec3 origin((k / 3) * 2.f, -(k % 3) * 2.f, 0.f);
		auto posFunc = [origin, easers, k](float x){ return origin + Math::Vec3(x, easers[k](x), 0.f); };
		STL::Add(s_posFunctions, posFunc);
		lines->GetMesh()->AddPointNoIndex(Graphics::PosColorVertex::MakeVertex(posFunc(0.f)));
		for (int i = 0; i < pointNum; ++i) {
			float x = 1.f / pointNum * i;
			lines->GetMesh()->AddLine(Graphics::PosColorVertex::MakeVertex(posFunc(x)));
		}
	}
	lines->GetMesh()->SubmitBuffer();

	s_points->GetMesh()->SetBufferType(Graphics::BufferType::Transient);

	entity->Add(s_points);
	entity->Add(lines);
	scene->Add(entity);
	scene->Add(renderer);
	SetScene(scene);
}

void EaseFunction::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}
