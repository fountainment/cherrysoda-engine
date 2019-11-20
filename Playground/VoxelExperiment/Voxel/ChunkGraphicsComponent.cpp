#include "ChunkGraphicsComponent.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>

namespace crsd = cherrysoda;
using crsd::Engine;
using crsd::Entity;
using crsd::Math;
using crsd::Color;
using crsd::Graphics;

void ChunkGraphicsComponent::Added(Entity* entity)
{
	m_chunk = static_cast<Chunk*>(entity);
	constexpr int chunkSize = Chunk::Size();
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if ((i * i + j * j + k * k) < 16)
					m_chunk->SetBlockType(i, j, k, Block::Type::White);
			}
		}
	}
}

void ChunkGraphicsComponent::EntityAwake()
{
	if (m_chunk == nullptr) return;

	constexpr int chunkSize = Chunk::Size();
	for (int i = 0; i < chunkSize; ++i) {
		for (int j = 0; j < chunkSize; ++j) {
			for (int k = 0; k < chunkSize; ++k) {
				if (m_chunk->GetBlockType(i, j, k) == Block::Type::White) {
					AddCube(Math::Vec3(i, j, k), 1.f, Color::White);
				}
			}
		}
	}
	m_mesh.InitBuffer();
}

void ChunkGraphicsComponent::Update()
{
	ZRotation(ZRotation() + Engine::Instance()->DeltaTime());
}

void ChunkGraphicsComponent::Render()
{
	Graphics::Instance()->SetTransformMatrix(GetTransformMatrix());
	Graphics::Instance()->SetVertexBuffer(m_mesh.GetVertexBuffer());
	Graphics::Instance()->SetIndexBuffer(m_mesh.GetIndexBuffer());
	Graphics::Instance()->Submit();
}

void ChunkGraphicsComponent::AddQuad(const Math::Vec3& pos, float size, const Color& color, const Math::Vec3& normal)
{
	auto makeVertex = [](const Math::Vec3& p, crsd::type::UInt32 c, const Math::Vec3& n)
	{
		return Graphics::PosColorNormalVertex{ p[0], p[1], p[2], c, n[0], n[1], n[2] };
	};
	const auto pVec = (Vec3_One - glm::abs(normal)) * size;
	const auto pVecH = normal[0] == 0.f ? Math::Vec3(size, 0.f, 0.f) : Math::Vec3(0.f, size, 0.f);
	const auto pVecV = normal[2] == 0.f ? Math::Vec3(0.f, 0.f, size) : Math::Vec3(0.f, size, 0.f);
	const auto cU32 = color.U32ABGR();
	m_mesh.AddQuad(
		makeVertex(pos + pVecV, cU32, normal),
		makeVertex(pos,         cU32, normal),
		makeVertex(pos + pVec,  cU32, normal),
		makeVertex(pos + pVecH, cU32, normal)
	);
}

void ChunkGraphicsComponent::AddCube(const Math::Vec3& pos, float size, const Color& color)
{
	AddQuad(pos + Vec3_XUp * size, size, color,  Vec3_XUp);
	AddQuad(pos                  , size, color, -Vec3_XUp);
	AddQuad(pos + Vec3_YUp * size, size, color,  Vec3_YUp);
	AddQuad(pos                  , size, color, -Vec3_YUp);
	AddQuad(pos + Vec3_ZUp * size, size, color,  Vec3_ZUp);
	AddQuad(pos                  , size, color, -Vec3_ZUp);
}