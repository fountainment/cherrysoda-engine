#include "VoxelExperiment.h"

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::Scene;

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

	SetScene(new Scene);
}