#include "VoxelExperiment.h"

#include <CherrySoda/Graphic/Graphic.h>
#include <CherrySoda/Utility/Color.h>

using voxelexperiment::VoxelExperiment;

using cherrysoda::ColorValue;
using cherrysoda::Graphic;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(ColorValue::Red);
}

void VoxelExperiment::Update()
{
	base::Update();

#ifdef CHERRYSODA_GLES2
	Graphic::TriangleGLES2();
#endif
}
