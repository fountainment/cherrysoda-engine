#include "World.h"

#include "Chunk.h"

#include <CherrySoda/Util/Math.h>

using cherrysoda::Math;

Chunk* World::LoadChunks()
{
	if (m_chunks) {
		delete [] m_chunks;
	}
	m_chunks = new Chunk[ChunkAmount()];
	for (int i = 0; i < Size(); ++i)
		for (int j = 0; j < Size(); ++j)
			for (int k = 0; k < Size(); ++k) {
				Chunk* chunk = GetChunk(Math::IVec3(i, j, k));
				chunk->Position(GetChunkPosition(Math::IVec3(i, j, k)));
				chunk->IndexVec3(i, j, k);
				chunk->m_world = this;
			}
	return m_chunks;	
}