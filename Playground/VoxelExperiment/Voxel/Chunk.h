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
	typedef cherrysoda::Entity base;	

	Chunk();

	void IndexVec3(int x, int y, int z) { m_chunkIndex = cherrysoda::Math::IVec3(x, y, z); }
	cherrysoda::Math::Vec3 IndexVec3() { return m_chunkIndex; }

	Block::Type GetBlockType(int x, int y, int z)
	{
		Block* block = GetBlock(x, y, z);
		return block ? block->m_type : Block::Type::None;
	}

	void SetBlockType(int x, int y, int z, Block::Type type);
	void RemoveBlock(int x, int y, int z) { SetBlockType(x, y, z, Block::Type::None); }
	void FillAllBlocks(Block::Type type);
	int GetBlockSurrounding(int x, int y, int z);

	void SetChanged();
	void SetChanged(int x, int y, int z);

	Block* GetBlocks() { return m_blocks.data(); }
	Block* GetBlock(int x, int y, int z);
	int GetBlockIndex(Block* block)
	{
		int index = static_cast<int>(block - GetBlocks());
		return (index < 0 || index >= BlockAmount()) ? -1 : index;
	}

	void Update() override;

	static constexpr int Size() { return static_cast<int>(ms_ChunkSize); }
	static constexpr int BlockAmount() { return Size() * Size() * Size(); }
	static constexpr cherrysoda::type::UInt16 ms_ChunkSize = 16; 

	World* m_world = nullptr;

private:
	cherrysoda::STL::Vector<Block> m_blocks;
	cherrysoda::Math::IVec3 m_chunkIndex = IVec3_Zero;

	bool m_changed = false;

	static int GetBlockIndex(int x, int y, int z)
	{ 
		if (x < 0 || x >= Size() || y < 0 || y >= Size() || z < 0 || z >= Size()) return -1;
		return z * Size() * Size() + y * Size() + x;
	}
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNK_H_