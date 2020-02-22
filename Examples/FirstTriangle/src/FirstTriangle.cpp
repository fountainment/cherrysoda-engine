#include "FirstTriangle.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Scene.h>

#include <CherrySoda/Components/Graphics/MeshGraphicsComponent.h>
#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Renderers/EverythingRenderer.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

using example::FirstTriangle;

using cherrysoda::Engine;
using cherrysoda::Entity;
using cherrysoda::Scene;

using cherrysoda::Color;
using cherrysoda::Effect;
using cherrysoda::EverythingRenderer;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::MeshGraphicsComponent;

class Triangle : public MeshGraphicsComponent<Graphics::PosColorVertex>
{
public:
	typedef MeshGraphicsComponent<Graphics::PosColorVertex> base;

	Triangle() : base(true)
	{
		GetMesh()->Clear();
		GetMesh()->AddTriangle(
			VertexType::MakeVertex(Math::Vec3( 0.0f,   1.0f, 0.0f), Color::Red.U32ABGR()),
			VertexType::MakeVertex(Math::Vec3(-0.87f, -0.5f, 0.0f), Color::Green.U32ABGR()),
			VertexType::MakeVertex(Math::Vec3( 0.87f, -0.5f, 0.0f), Color::Blue.U32ABGR())
		);
		GetMesh()->InitBuffer();
	}

	void Update() override
	{
		ZRotation(ZRotation() + Engine::Instance()->DeltaTime());
	}
};

FirstTriangle::FirstTriangle()
	: base()
{
	SetTitle("FirstTriangle");
	SetClearColor(Color::Black);
}

void FirstTriangle::Initialize()
{
	base::Initialize();

	ms_shader.Load("basic");

	auto scene = new Scene();
	auto entity = new Entity();

	auto triangle = new Triangle();
	auto renderer = new EverythingRenderer();

	entity->Add(triangle);
	scene->Add(entity);
	
	renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 4.f));
	renderer->SetEffect(ms_shader);
	scene->Add(renderer);

	SetScene(scene);
}

Effect FirstTriangle::ms_shader;