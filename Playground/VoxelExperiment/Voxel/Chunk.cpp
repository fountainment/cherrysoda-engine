#include "Chunk.h"

#include "ChunkGraphicsComponent.h"

using cherrysoda::STL;

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlockAmount());
	STL::Fill(m_blocks, Block());

	Add(new ChunkGraphicsComponent);
}