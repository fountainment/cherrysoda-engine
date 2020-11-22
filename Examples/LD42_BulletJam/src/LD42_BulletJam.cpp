#include "LD42_BulletJam.h"

#include <CherrySoda/CherrySoda.h>

#include "BulletJamScene.h"

using ld42_bulletjam::LD42_BulletJam;
using ld42_bulletjam::BulletJamScene;

using namespace cherrysoda;

static Atlas s_atlas;

LD42_BulletJam::LD42_BulletJam()
	: base(600, 700, 600, 700, "BulletJam", false)
{
	SetClearColor(Color::Black);
}

void LD42_BulletJam::Update()
{
	base::Update();

	// Add global GUI or other global stuffs here
}

void LD42_BulletJam::Initialize()
{
	base::Initialize();

	// Initialize and set scene here
	auto scene = new BulletJamScene();
	SetScene(scene);
}

void LD42_BulletJam::LoadContent()
{
	base::LoadContent();

	// Load textures, sprites, shaders and other resources here
	s_atlas = Atlas::FromAtlas("assets/atlases/atlas.json");
}

Atlas* LD42_BulletJam::GetAtlas()
{
	return &s_atlas;
}
