#include "Chunk.h"

#include "ChunkGraphicsComponent.h"

using cherrysoda::STL;

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlockAmount());
	STL::Fill(m_blocks, Block());

	Add(new ChunkGraphicsComponent);
}

int Chunk::GetBlockSurrounding(int x, int y, int z)
{
	int result = 0;
	if (GetBlockType(x + 1, y, z) == Block::Type::None) {
		result |= 1;
	}
	if (GetBlockType(x - 1, y, z) == Block::Type::None) {
		result |= 1 << 1;
	}
	if (GetBlockType(x, y + 1, z) == Block::Type::None) {
		result |= 1 << 2;
	}
	if (GetBlockType(x, y - 1, z) == Block::Type::None) {
		result |= 1 << 3;
	}
	if (GetBlockType(x, y, z + 1) == Block::Type::None) {
		result |= 1 << 4;
	}
	if (GetBlockType(x, y, z - 1) == Block::Type::None) {
		result |= 1 << 5;
	}
	return result;
}

Block Chunk::ms_noneBlock;