#ifndef _VOXELEXPERIMENT_VOXEL_CHUNK_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNK_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/NumType.h>

#include "Voxel/Block.h"

class World;
class ChunkGraphicsComponent;

class Chunk : public cherrysoda::Entity
{
public:
	typedef cherrysoda::Entity base;	

	Chunk();

	static constexpr int Size() { return static_cast<int>(ms_ChunkSize); }
	static constexpr int BlockAmount() { return Size() * Size() * Size(); }

	void IndexVec3(int x, int y, int z) { m_chunkIndex = cherrysoda::Math::IVec3(x, y, z); }
	cherrysoda::Math::Vec3 IndexVec3() { return m_chunkIndex; }

	Block::Type GetBlockType(const cherrysoda::Math::IVec3& v)
	{
		Block* block = GetBlock(v);
		return block ? block->m_type : Block::Type::None;
	}
	void SetBlockType(const cherrysoda::Math::IVec3& v, Block::Type type);

	void RemoveBlock(const cherrysoda::Math::IVec3& v) { SetBlockType(v, Block::Type::None); }
	void FillAllBlocks(Block::Type type);
	int GetBlockSurrounding(const cherrysoda::Math::IVec3& v);

	void SetChanged();
	void SetChanged(const cherrysoda::Math::IVec3& v);

	Block* GetBlocks() { return m_blocks.data(); }
	Block* GetBlock(const cherrysoda::Math::IVec3& v);
	int GetBlockIndex(Block* block)
	{
		int index = static_cast<int>(block - GetBlocks());
		return (index < 0 || index >= BlockAmount()) ? -1 : index;
	}

	inline cherrysoda::Math::AABB GetAABB() const { return { Position(), Position() + cherrysoda::Math::Vec3(static_cast<float>(Size())) }; }
	inline cherrysoda::Math::AABB GetBlockAABB(const cherrysoda::Math::IVec3& v) const { return { Position() + cherrysoda::Math::Vec3(v), Position() + cherrysoda::Math::Vec3(v) + Vec3_One }; }

	void Update() override;

	World* m_world = nullptr;

private:
	cherrysoda::STL::Vector<Block> m_blocks;
	cherrysoda::Math::IVec3 m_chunkIndex = IVec3_Zero;

	ChunkGraphicsComponent* m_chunkGraphicsComponent;
	bool m_changed = false;

	static constexpr cherrysoda::type::UInt16 ms_ChunkSize = 16;

	static inline int GetBlockIndex(const cherrysoda::Math::IVec3& v)
	{ 
		if (v[0] < 0 || v[0] >= Size() || v[1] < 0 || v[1] >= Size() || v[2] < 0 || v[2] >= Size()) return -1;
		return v[2] * Size() * Size() + v[1] * Size() + v[0];
	}

public:
	CHERRYSODA_ITERABLE(m_blocks);
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNK_H_
