#include "VoxelExperiment.h"

#include "Scenes/MainScene.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::Texture;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetTitle("VoxelExperiment");
	SetClearColor(Color::Black);
}

void VoxelExperiment::Update()
{
	base::Update();
}

void VoxelExperiment::Initialize()
{
	base::Initialize();

	auto scene = new MainScene();
	SetScene(scene);
}

void VoxelExperiment::LoadContent()
{
	ms_texCube = Texture::FromFile("assets/textures/bolonga_lod.dds");
	ms_texCubeIrr = Texture::FromFile("assets/textures/bolonga_irr.dds");
}

Texture VoxelExperiment::ms_texCube;
Texture VoxelExperiment::ms_texCubeIrr;