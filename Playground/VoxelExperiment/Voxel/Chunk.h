#ifndef _VOXELEXPERIMENT_VOXEL_CHUNK_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNK_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/NumType.h>

#include "Voxel/Block.h"

class World;

class Chunk : public cherrysoda::Entity
{
public:
	Chunk();

	Block::Type GetBlockType(int x, int y, int z)
	{
		Block* block = GetBlock(x, y, z);
		return block ? block->m_type : Block::Type::None;
	}
	void SetBlockType(int x, int y, int z, Block::Type type)
	{
		Block* block = GetBlock(x, y, z);
		if (block && block->m_type != type) {
			block->m_type = type;
			m_changed = true;
		}
	}
	void RemoveBlock(int x, int y, int z) { SetBlockType(x, y, z, Block::Type::None); }
	void FillAllBlocks(Block::Type type);
	int GetBlockSurrounding(int x, int y, int z);

	Block* GetBlocks() { return m_blocks.data(); }
	Block* GetBlock(int x, int y, int z);
	int GetBlockIndex(Block* block)
	{
		int index = block - GetBlocks();
		return (index < 0 || index >= BlocksAmount()) ? -1 : index;
	}

	void Update() override;

	static constexpr int Size() { return static_cast<int>(ms_ChunkSize); }
	static constexpr int BlocksAmount() { return Size() * Size() * Size(); }
	static constexpr cherrysoda::type::UInt16 ms_ChunkSize = 16; 

	World* m_world = nullptr;
	bool m_changed = false;

private:
	cherrysoda::STL::Vector<Block> m_blocks;

	static int GetBlockIndex(int x, int y, int z)
	{ 
		if (x < 0 || x >= Size() || y < 0 || y >= Size() || z < 0 || z >= Size()) return -1;
		return z * Size() * Size() + y * Size() + x;
	}
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNK_H_