#include "Chunk.h"

#include "Voxel/World.h"

#include "ChunkGraphicsComponent.h"

#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::Math;
using cherrysoda::STL;

static constexpr Math::IVec3 s_offset[6] = { IVec3_XUp, -IVec3_XUp, IVec3_YUp, -IVec3_YUp, IVec3_ZUp, -IVec3_ZUp };

Chunk::Chunk()
{
	STL::Resize(m_blocks, BlockAmount());
	STL::Fill(m_blocks, Block());

	m_chunkGraphicsComponent = new ChunkGraphicsComponent;
	Add(m_chunkGraphicsComponent);
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
	for (int i = 0; i < 6; ++i) {
		if (GetBlockType(v + s_offset[i]) == Block::Type::None) {
			result |= 1 << i;
		}
	}
	return result;
}


void Chunk::SetChanged()
{
	m_changed = true;

	if (m_world) {
		for (int i = 0; i < 6; ++i) {
			Chunk* chunk = m_world->GetChunk(m_chunkIndex + s_offset[i]);
			if (chunk) {
				chunk->m_changed = true;
			}
		}
	}
}


void Chunk::SetChanged(const Math::IVec3& v)
{
	m_changed = true;

	if (m_world) {
		Chunk* chunks[6];
		for (int i = 0; i < 3; ++i) {
			chunks[i << 1]       = (v[i] == Size() - 1) ? m_world->GetChunk(m_chunkIndex + s_offset[i << 1])       : nullptr;
			chunks[(i << 1) | 1] = (v[i] == 0)          ? m_world->GetChunk(m_chunkIndex + s_offset[(i << 1) | 1]) : nullptr;
		}
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
		m_chunkGraphicsComponent->RebuildMesh();
		m_changed = false;
	}
}
