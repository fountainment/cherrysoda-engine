#ifndef _VOXELEXPERIMENT_VOXEL_WORLD_H_
#define _VOXELEXPERIMENT_VOXEL_WORLD_H_

#include "Voxel/Chunk.h"

#include <CherrySoda/Util/Math.h>

class World
{
public:
	World(cherrysoda::Math::Vec3 basePosition = Vec3_Zero)
	: m_basePosition(basePosition)
	{
	}

	Chunk* LoadChunks();

	inline int GetChunkIndex(Chunk* chunk)
	{
		int index = static_cast<int>(chunk - GetChunks());
		return (index >= 0 && index < ChunkAmount()) ? index : -1;
	}

	inline Chunk* GetChunks() { return m_chunks; }
	inline Chunk* GetChunk(const cherrysoda::Math::IVec3& v)
	{
		int index = GetChunkIndex(v);
		return index >= 0 ? GetChunks() + index : nullptr;
	}

	Block* GetBlock(const cherrysoda::Math::IVec3& v, Chunk** chunkOut = nullptr)
	{
		int worldBlockSize = WorldBlockSize();
		if (v[0] < 0 || v[0] >= worldBlockSize || v[1] < 0 || v[1] >= worldBlockSize || v[2] < 0 || v[2] >=worldBlockSize) {
			return nullptr;
		}
		Chunk* chunk = GetChunk(v / Chunk::Size());
		if (chunk && chunkOut) *chunkOut = chunk;
		return chunk ? chunk->GetBlock(v % Chunk::Size()) : nullptr;
	}

	void SetBlockType(const cherrysoda::Math::IVec3& v, Block::Type type)
	{
		Chunk* chunk = nullptr;
		Block* block = GetBlock(v, &chunk);
		if (block && block->m_type != type)
		{
			block->m_type = type;
			chunk->SetChanged(v % Chunk::Size());
		}	
	}

	void SetBlockType(int index, Block::Type type)
	{
		int worldBlockSize = WorldBlockSize();
		int x = index;
		int z = x % worldBlockSize;
		x /= worldBlockSize;
		int y = x % worldBlockSize;
		x /= worldBlockSize;
		SetBlockType(cherrysoda::Math::IVec3(x, y, z), type);
	}

	inline cherrysoda::Math::Vec3 BasePosition() const { return m_basePosition; }
	inline cherrysoda::Math::Vec3 GetChunkPosition(const cherrysoda::Math::IVec3& v) const { return BasePosition() + GetWorldChunkPosition(v); }
	inline cherrysoda::Math::Vec3 GetBlockPosition(const cherrysoda::Math::IVec3& v) const { return BasePosition() + cherrysoda::Math::Vec3(v); }

	static constexpr int Size() { return static_cast<int>(ms_worldSize); }
	static constexpr int WorldBlockSize() { return Size() * Chunk::Size(); }
	static constexpr int ChunkAmount() { return Size() * Size() * Size(); }
	static constexpr int BlockAmount() { return ChunkAmount() * Chunk::BlockAmount(); }

	static inline const cherrysoda::Math::Vec3 GetWorldChunkPosition(const cherrysoda::Math::IVec3& v) { return cherrysoda::Math::Vec3(v * Chunk::Size()); }

private:
	Chunk* m_chunks = nullptr;
	cherrysoda::Math::Vec3 m_basePosition;

	static inline int GetChunkIndex(const cherrysoda::Math::IVec3& v)
	{
		if (v[0] < 0 || v[0] >= Size() || v[1] < 0 || v[1] >= Size() || v[2] < 0 || v[2] >= Size()) return -1;
		return v[2] * Size() * Size() + v[1] * Size() + v[0];
	}
	
	static constexpr int ms_worldSize = 8; 
};

#endif // _VOXELEXPERIMENT_VOXEL_WORLD_H_