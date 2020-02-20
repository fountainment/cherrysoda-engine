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
	inline Chunk* GetChunk(int x, int y, int z)
	{
		int index = GetChunkIndex(x, y, z);
		return index >= 0 ? GetChunks() + index : nullptr;
	}
	Block* GetBlock(int x, int y, int z, Chunk** chunkOut = nullptr)
	{
		int worldBlockSize = WorldBlockSize();
		if (x < 0 || x >= worldBlockSize || y < 0 || y >= worldBlockSize || z < 0 || z >=worldBlockSize) {
			return nullptr;
		}
		Chunk* chunk = GetChunk(x / Chunk::Size(), y / Chunk::Size(), z / Chunk::Size());
		if (chunk && chunkOut) *chunkOut = chunk;
		return chunk ? chunk->GetBlock(x % Chunk::Size(), y % Chunk::Size(), z % Chunk::Size()) : nullptr;
	}

	void SetBlockType(int x, int y, int z, Block::Type type)
	{
		Chunk* chunk = nullptr;
		Block* block = GetBlock(x, y, z, &chunk);
		if (block && block->m_type != type)
		{
			block->m_type = type;
			chunk->SetChanged(x % Chunk::Size(), y % Chunk::Size(), z % Chunk::Size());
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
		SetBlockType(x, y, z, type);
	}

	inline cherrysoda::Math::Vec3 BasePosition() const { return m_basePosition; }
	inline cherrysoda::Math::Vec3 GetChunkPosition(int x, int y, int z) const { return BasePosition() + GetWorldChunkPosition(x, y, z); }
	inline cherrysoda::Math::Vec3 GetBlockPosition(int x, int y, int z) const { return BasePosition() + cherrysoda::Math::Vec3(x, y, z); }

	static constexpr int Size() { return static_cast<int>(ms_worldSize); }
	static constexpr int WorldBlockSize() { return Size() * Chunk::Size(); }
	static constexpr int ChunkAmount() { return Size() * Size() * Size(); }
	static constexpr int BlockAmount() { return ChunkAmount() * Chunk::BlockAmount(); }
	static inline const cherrysoda::Math::Vec3 GetWorldChunkPosition(int x, int y, int z) { return cherrysoda::Math::Vec3(x * Chunk::Size(), y * Chunk::Size(), z * Chunk::Size()); }

private:
	Chunk* m_chunks = nullptr;
	cherrysoda::Math::Vec3 m_basePosition;

	static inline int GetChunkIndex(int x, int y, int z)
	{
		if (x < 0 || x >= Size() || y < 0 || y >= Size() || z < 0 || z >= Size()) return -1;
		return z * Size() * Size() + y * Size() + x;
	}
	
	static constexpr int ms_worldSize = 8; 
};

#endif // _VOXELEXPERIMENT_VOXEL_WORLD_H_