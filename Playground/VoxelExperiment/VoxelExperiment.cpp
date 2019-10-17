#include "VoxelExperiment.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Utility/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Graphics;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(Color::Red);
}

void VoxelExperiment::Update()
{
	base::Update();
}