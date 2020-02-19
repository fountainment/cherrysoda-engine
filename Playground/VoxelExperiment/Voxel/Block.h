#ifndef _VOXELEXPERIMENT_VOXEL_BLOCK_H_
#define _VOXELEXPERIMENT_VOXEL_BLOCK_H_

struct Block
{
	enum class Type
	{
		None   = 0,
		White  = 1,
		Black  = 2,
	};

	Type m_type = Type::None;
};

#endif // _VOXELEXPERIMENT_VOXEL_BLOCK_H_