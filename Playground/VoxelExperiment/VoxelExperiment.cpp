#include "VoxelExperiment.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Utility/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::ColorValue;
using cherrysoda::Graphics;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(ColorValue::Red);
}

void VoxelExperiment::Update()
{
	base::Update();

#ifdef CHERRYSODA_GLES2
	Graphics::TriangleGLES2();
#endif
}