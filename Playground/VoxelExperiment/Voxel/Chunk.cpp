#include "Chunk.h"

#include "Voxel/World.h"

#include "ChunkGraphicsComponent.h"

using cherrysoda::STL;

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlocksAmount());
	STL::Fill(m_blocks, Block());

	Add(new ChunkGraphicsComponent);
}

void Chunk::SetBlockType(int x, int y, int z, Block::Type type)
{
	Block* block = GetBlock(x, y, z);
	if (block && block->m_type != type) {
		block->m_type = type;
		SetChanged();
	}
}

void Chunk::FillAllBlocks(Block::Type type)
{
	bool changed = false;
	for (int i = 0; i < BlocksAmount(); ++i)
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
		int x = static_cast<int>(Position()[0] / Size());
		int y = static_cast<int>(Position()[1] / Size());
		int z = static_cast<int>(Position()[2] / Size());
		Chunk* chunks[6];
		chunks[0] = m_world->GetChunk(x - 1, y, z);
		chunks[1] = m_world->GetChunk(x + 1, y, z);
		chunks[2] = m_world->GetChunk(x, y - 1, z);
		chunks[3] = m_world->GetChunk(x, y + 1, z);
		chunks[4] = m_world->GetChunk(x, y, z - 1);
		chunks[5] = m_world->GetChunk(x, y, z + 1);
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
			int wx = x + static_cast<int>(Position()[0] + 0.5f);
			int wy = y + static_cast<int>(Position()[1] + 0.5f);
			int wz = z + static_cast<int>(Position()[2] + 0.5f);
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