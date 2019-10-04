#include "VoxelExperiment.h"

#include <CherrySoda/Utility/Color.h>

using voxelexperiment::VoxelExperiment;
using cherrysoda::ColorValue;

VoxelExperiment::VoxelExperiment()
	: base()
{
	SetClearColor(ColorValue::White);
}