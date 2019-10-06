#include "VoxelExperiment.h"

#include <CherrySoda/Utility/Color.h>

using voxelexperiment::VoxelExperiment;
using cherrysoda::ColorValue;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(ColorValue::White);
}

void VoxelExperiment::Update()
{
	base::Update();

#ifdef CHERRYSODA_GLES2
	Graphic::TriangleGLES2();
#endif
}