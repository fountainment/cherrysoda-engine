#include "FirstModel.h"

#include <CherrySoda/CherrySoda.h>

using firstmodel::FirstModel;

using namespace cherrysoda;

CHERRYSODA_CONSOLE_VARIABLE(rotation_speed, float, 0.35f, "");

class ModelMesh : public MeshGraphicsComponent<Graphics::PosNormalTexCoord0Vertex>
{
public:
	CHERRYSODA_DECLARE_COMPONENT(ModelMesh, MeshGraphicsComponent<Graphics::PosNormalTexCoord0Vertex>);

	ModelMesh() : base(true) {}

	void Update() override
	{
		base::Update();

		ZRotation(Engine::Instance()->GameTime() * rotation_speed);
	}
};

STL::Vector<STL::Action<>> s_updaters;

FirstModel::FirstModel()
	: base()
{
	SetTitle("FirstModel");
	SetClearColor(Color::Black);
}

void FirstModel::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void FirstModel::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new Scene();
	auto renderer = new EverythingRenderer();

	auto camera = renderer->GetCamera();
	camera->SetUpVector(Vec3_ZUp);
	camera->Position(Math::Vec3(0.0f, 2.0f, 0.0f));
	camera->Direction(Math::Vec3(0.0f, -1.0f, 0.0f));

	renderer->KeepCameraCenterOrigin(true);
	renderer->SetEffect(Graphics::GetEmbeddedEffect("model"));
	scene->Add(renderer);

	m_model = Model::FromGltf("assets/models/cake/scene.gltf");

	auto entity = new Entity();
	for (auto& mesh : m_model.Meshes()) {
		auto meshComp = new ModelMesh();
		meshComp->InitWithMeshInfo(mesh);
		meshComp->Scale(Math::Vec3(10.f));
		meshComp->Origin(Math::Vec3(0.0f, -0.04f, 0.0f));
		entity->Add(meshComp);
	}
	scene->Add(entity);

	SetScene(scene);
}

void FirstModel::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
}
