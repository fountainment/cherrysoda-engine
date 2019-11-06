#include "VoxelExperiment.h"

#include "Scenes/MainScene.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Graphics;

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