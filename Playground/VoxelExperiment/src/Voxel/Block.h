#ifndef _VOXELEXPERIMENT_VOXEL_BLOCK_H_
#define _VOXELEXPERIMENT_VOXEL_BLOCK_H_

#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

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

	inline int GetHash() const { return static_cast<int>(m_type); }

	Type m_type = Type::None;
};

CHERRYSODA_HASHABLE(Block,GetHash);

#endif // _VOXELEXPERIMENT_VOXEL_BLOCK_H_
