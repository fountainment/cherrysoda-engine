#include "Chunk.h"

#include "Voxel/World.h"

#include "ChunkGraphicsComponent.h"

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Math;
using cherrysoda::STL;

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlockAmount());
	STL::Fill(m_blocks, Block());

	Add(new ChunkGraphicsComponent);
}

void Chunk::SetBlockType(int x, int y, int z, Block::Type type)
{
	Block* block = GetBlock(x, y, z);
	if (block && block->m_type != type) {
		block->m_type = type;
		SetChanged(x, y, z);
	}
}

void Chunk::FillAllBlocks(Block::Type type)
{
	bool changed = false;
	for (int i = 0; i < BlockAmount(); ++i)
	{
		if (GetBlocks()[i].m_type != type) {
			GetBlocks()[i].m_type = type;
			changed = true;
		}
		
	}
	if (changed) {
		SetChanged();
	}
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


void Chunk::SetChanged()
{
	m_changed = true;

	if (m_world) {
		int cx = m_chunkIndex[0];
		int cy = m_chunkIndex[1];
		int cz = m_chunkIndex[2];
		Chunk* chunks[6];
		chunks[0] = m_world->GetChunk(cx - 1, cy, cz);
		chunks[1] = m_world->GetChunk(cx + 1, cy, cz);
		chunks[2] = m_world->GetChunk(cx, cy - 1, cz);
		chunks[3] = m_world->GetChunk(cx, cy + 1, cz);
		chunks[4] = m_world->GetChunk(cx, cy, cz - 1);
		chunks[5] = m_world->GetChunk(cx, cy, cz + 1);
		for (int i = 0; i < 6; ++i) {
			if (chunks[i]) {
				chunks[i]->m_changed = true;
			}
		}
	}
}


void Chunk::SetChanged(int x, int y, int z)
{
	m_changed = true;

	if (m_world) {
		int cx = m_chunkIndex[0];
		int cy = m_chunkIndex[1];
		int cz = m_chunkIndex[2];
		Chunk* chunks[6];
		chunks[0] = (x == 0)          ? m_world->GetChunk(cx - 1, cy, cz) : nullptr;
		chunks[1] = (x == Size() - 1) ? m_world->GetChunk(cx + 1, cy, cz) : nullptr;
		chunks[2] = (y == 0)          ? m_world->GetChunk(cx, cy - 1, cz) : nullptr;
		chunks[3] = (y == Size() - 1) ? m_world->GetChunk(cx, cy + 1, cz) : nullptr;
		chunks[4] = (z == 0)          ? m_world->GetChunk(cx, cy, cz - 1) : nullptr;
		chunks[5] = (z == Size() - 1) ? m_world->GetChunk(cx, cy, cz + 1) : nullptr;
		for (int i = 0; i < 6; ++i) {
			if (chunks[i]) {
				chunks[i]->m_changed = true;
			}
		}
	}
}

Block* Chunk::GetBlock(int x, int y, int z)
{
	int index = GetBlockIndex(x, y, z); 
	if (index < 0) {
		if (m_world) {
			Math::IVec3 blockIndex = m_chunkIndex * Chunk::Size();
			int wx = x + blockIndex[0];
			int wy = y + blockIndex[1];
			int wz = z + blockIndex[2];
			return m_world->GetBlock(wx, wy, wz);
		}
		return nullptr;
	}
	return GetBlocks() + index;
}

void Chunk::Update()
{
	base::Update();

	if (m_changed) {
		Get<ChunkGraphicsComponent>()->RebuildMesh();
		m_changed = false;
	}
}