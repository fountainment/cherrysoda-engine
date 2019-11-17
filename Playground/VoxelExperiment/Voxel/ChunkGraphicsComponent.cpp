#include "ChunkGraphicsComponent.h"

#include <CherrySoda/Graphics/Graphics.h>

using cherrysoda::Entity;
using cherrysoda::Graphics;

void ChunkGraphicsComponent::Added(Entity* entity)
{
	m_chunk = static_cast<Chunk*>(entity);
}

void ChunkGraphicsComponent::Update()
{
	if (m_chunk == nullptr) return;

	constexpr int chunkSize = Chunk::Size();
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (m_chunk->GetBlockType(i, j, k) == Block::Type::White) {

				}
			}
		}
	}
}

void ChunkGraphicsComponent::Render()
{
	Graphics::Instance()->SetTransformMatrix(GetTransformMatrix());
	Graphics::Instance()->Submit();
}