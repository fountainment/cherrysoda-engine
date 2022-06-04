#include "FirstTriangle.h"

#include <CherrySoda/CherrySoda.h>

using example::FirstTriangle;

using namespace cherrysoda;

class Triangle : public MeshGraphicsComponent<Graphics::PosColorVertex>
{
public:
	CHERRYSODA_DECLARE_COMPONENT(Triangle, MeshGraphicsComponent<VertexType>);

	Triangle() : base(true)
	{
		// Currently mesh in this engine has only one primitive
		GetMesh()->SetPrimitiveType(Graphics::PrimitiveType::Triangles);

		GetMesh()->Clear();
		GetMesh()->AddTriangle(
			MK_VERT(Math::Vec3( 0.0f,   1.0f, 0.0f), Color::Red),
			MK_VERT(Math::Vec3(-0.87f, -0.5f, 0.0f), Color::Green),
			MK_VERT(Math::Vec3( 0.87f, -0.5f, 0.0f), Color::Blue)
		);
		GetMesh()->SubmitBuffer();
	}

	void Update() override
	{
		RotateOnZ(Engine::Instance()->DeltaTime());
	}
};

FirstTriangle::FirstTriangle() : base()
{
	SetTitle("FirstTriangle");
	SetClearColor(Color::Black);
}

void FirstTriangle::Initialize()
{
	base::Initialize();

	auto scene = new Scene();
	auto entity = new Entity();

	auto triangle = new Triangle();
	auto renderer = new EverythingRenderer();

	entity->Add(triangle);
	scene->Add(entity);

	renderer->GetCamera()->Position(Math::Vec3(0.f, 0.f, 4.f));
	scene->Add(renderer);

	SetScene(scene);
}
