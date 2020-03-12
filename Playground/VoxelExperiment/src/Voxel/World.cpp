#include "World.h"

#include "Chunk.h"

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Math;
using cherrysoda::STL;

Chunk* World::LoadChunks()
{
	STL::Resize(m_chunks, ChunkAmount());
	for (int i = 0; i < Size(); ++i)
		for (int j = 0; j < Size(); ++j)
			for (int k = 0; k < Size(); ++k) {
				Chunk* chunk = GetChunk(Math::IVec3(i, j, k));
				chunk->Position(GetChunkPosition(Math::IVec3(i, j, k)));
				chunk->IndexVec3(i, j, k);
				chunk->m_world = this;
			}
	return STL::Data(m_chunks);
}
