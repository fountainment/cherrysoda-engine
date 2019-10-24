#include "VoxelExperiment.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::Color;
using cherrysoda::Graphics;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(Color::Black);
}

void VoxelExperiment::Update()
{
	base::Update();
}