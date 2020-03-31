#ifndef _VOXELEXPERIMENT_VOXEL_BLOCK_H_
#define _VOXELEXPERIMENT_VOXEL_BLOCK_H_

#include <CherrySoda/Util/NumType.h>

struct Block
{
	enum class Type : cherrysoda::type::UInt8
	{
		None   = 0,
		White  = 1,
		Black  = 2,
		Red    = 3,
		Green  = 4,
		Blue   = 5
	};

	Type m_type = Type::None;
};

#endif // _VOXELEXPERIMENT_VOXEL_BLOCK_H_
