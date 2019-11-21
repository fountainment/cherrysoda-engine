#ifndef _VOXELEXPERIMENT_VOXEL_CHUNK_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNK_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/NumType.h>

#include "Block.h"

class Chunk : public cherrysoda::Entity
{
public:
	Chunk();

	Block::Type GetBlockType(int x, int y, int z) { return GetBlock(x, y, z)->m_type; };
	void SetBlockType(int x, int y, int z, Block::Type type) { GetBlock(x, y, z)->m_type = type; };
	void RemoveBlock(int x, int y, int z) { SetBlockType(x, y, z, Block::Type::None); }
	int GetBlockSurrounding(int x, int y, int z);

	static constexpr int Size() { return static_cast<int>(ms_ChunkSize); }
	static constexpr int BlockAmount() { return Size() * Size() * Size(); }
	static constexpr cherrysoda::type::UInt16 ms_ChunkSize = 16; 

private:
	cherrysoda::STL::Vector<Block> m_blocks;

	Block* GetBlocks() { return m_blocks.data(); }
	Block* GetBlock(int x, int y, int z)
	{
		if (x < 0 || x >= Size() || y < 0 || y >= Size() || z < 0 || z >= Size())
			return &ms_noneBlock;
		return GetBlocks() + GetIndex(x, y, z);
	}

	static int GetIndex(int x, int y, int z) { return z * Size() * Size() + y * Size() + x; }
	static Block ms_noneBlock;
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNK_H_