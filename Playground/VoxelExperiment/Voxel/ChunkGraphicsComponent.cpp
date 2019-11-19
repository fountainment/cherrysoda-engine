#include "ChunkGraphicsComponent.h"

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Entity;
using cherrysoda::Math;
using cherrysoda::Graphics;

void ChunkGraphicsComponent::Added(Entity* entity)
{
	m_chunk = static_cast<Chunk*>(entity);
	m_chunk->SetBlockType(0, 0, 0, Block::Type::White);
}

void ChunkGraphicsComponent::EntityAwake()
{
	if (m_chunk == nullptr) return;

	constexpr int chunkSize = Chunk::Size();
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (m_chunk->GetBlockType(i, j, k) == Block::Type::White) {
					m_mesh.AddTriangle(
						{0.0f, 0.0f, 0.0f, 0xffff0000, 0.f, 0.f, 1.f},
						{0.0f, 1.0f, 0.0f, 0xff0000ff, 0.f, 0.f, 1.f},
						{-1.0f, 0.0f, 0.0f, 0xff00ffff, 0.f, 0.f, 1.f}
					);	
				}
			}
		}
	}
	m_mesh.InitBuffer();
}

void ChunkGraphicsComponent::Render()
{
	Graphics::Instance()->SetTransformMatrix(GetTransformMatrix());
	Graphics::Instance()->SetVertexBuffer(m_mesh.GetVertexBuffer());
	Graphics::Instance()->SetIndexBuffer(m_mesh.GetIndexBuffer());
	Graphics::Instance()->Submit();
}