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

void Chunk::SetBlockType(const Math::IVec3& v, Block::Type type)
{
	Block* block = GetBlock(v);
	if (block && block->m_type != type) {
		block->m_type = type;
		SetChanged(v);
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

int Chunk::GetBlockSurrounding(const Math::IVec3& v)
{
	int result = 0;
	if (GetBlockType(v + IVec3_XUp) == Block::Type::None) {
		result |= 1;
	}
	if (GetBlockType(v - IVec3_XUp) == Block::Type::None) {
		result |= 1 << 1;
	}
	if (GetBlockType(v + IVec3_YUp) == Block::Type::None) {
		result |= 1 << 2;
	}
	if (GetBlockType(v - IVec3_YUp) == Block::Type::None) {
		result |= 1 << 3;
	}
	if (GetBlockType(v + IVec3_ZUp) == Block::Type::None) {
		result |= 1 << 4;
	}
	if (GetBlockType(v - IVec3_ZUp) == Block::Type::None) {
		result |= 1 << 5;
	}
	return result;
}


void Chunk::SetChanged()
{
	m_changed = true;

	if (m_world) {
		Chunk* chunks[6];
		chunks[0] = m_world->GetChunk(m_chunkIndex - IVec3_XUp);
		chunks[1] = m_world->GetChunk(m_chunkIndex + IVec3_XUp);
		chunks[2] = m_world->GetChunk(m_chunkIndex - IVec3_YUp);
		chunks[3] = m_world->GetChunk(m_chunkIndex + IVec3_YUp);
		chunks[4] = m_world->GetChunk(m_chunkIndex - IVec3_ZUp);
		chunks[5] = m_world->GetChunk(m_chunkIndex + IVec3_ZUp);
		for (int i = 0; i < 6; ++i) {
			if (chunks[i]) {
				chunks[i]->m_changed = true;
			}
		}
	}
}


void Chunk::SetChanged(const Math::IVec3& v)
{
	m_changed = true;

	if (m_world) {
		Chunk* chunks[6];
		chunks[0] = (v[0] == 0)          ? m_world->GetChunk(m_chunkIndex - IVec3_XUp) : nullptr;
		chunks[1] = (v[0] == Size() - 1) ? m_world->GetChunk(m_chunkIndex + IVec3_XUp) : nullptr;
		chunks[2] = (v[1] == 0)          ? m_world->GetChunk(m_chunkIndex - IVec3_YUp) : nullptr;
		chunks[3] = (v[1] == Size() - 1) ? m_world->GetChunk(m_chunkIndex + IVec3_YUp) : nullptr;
		chunks[4] = (v[2] == 0)          ? m_world->GetChunk(m_chunkIndex - IVec3_ZUp) : nullptr;
		chunks[5] = (v[2] == Size() - 1) ? m_world->GetChunk(m_chunkIndex + IVec3_ZUp) : nullptr;
		for (int i = 0; i < 6; ++i) {
			if (chunks[i]) {
				chunks[i]->m_changed = true;
			}
		}
	}
}

Block* Chunk::GetBlock(const Math::IVec3& v)
{
	int index = GetBlockIndex(v); 
	if (index < 0) {
		if (m_world) {
			Math::IVec3 baseBlockIndex = m_chunkIndex * Chunk::Size();
			return m_world->GetBlock(baseBlockIndex + v);
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