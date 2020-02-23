#include "VoxelExperiment.h"

#include "Scenes/MainScene.h"

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Effect;
using cherrysoda::Graphics;
using cherrysoda::TextureCube;

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
	ms_texCube = TextureCube::FromFile("assets/textures/bolonga_lod.dds");
	ms_texCubeIrr = TextureCube::FromFile("assets/textures/bolonga_irr.dds");

	ms_voxelShader.Load("mypbr");
	ms_skyboxShader.Load("skybox");
}

TextureCube VoxelExperiment::ms_texCube;
TextureCube VoxelExperiment::ms_texCubeIrr;

Effect VoxelExperiment::ms_voxelShader;
Effect VoxelExperiment::ms_skyboxShader;
