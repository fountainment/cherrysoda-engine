#include "ChunkGraphicsComponent.h"

#include "Program.h"

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace crsd = cherrysoda;
using crsd::Engine;
using crsd::Entity;
using crsd::Math;
using crsd::MeshInterface;
using crsd::Color;
using crsd::Graphics;
using crsd::STL;

void ChunkGraphicsComponent::EntityAwake()
{
	GetMesh()->SetBufferType(Graphics::BufferType::Dynamic);
	RebuildMesh();
}

void ChunkGraphicsComponent::RebuildMesh()
{
	CHERRYSODA_PROFILE_FUNCTION();

	static Color blockTypeColorMap[(int)Block::Type::Count] = {
		Color::White,
		Color::White,
		Color::Black,
		Color::Red,
		Color::Green,
		Color::Blue
	};

	constexpr int chunkSize = Chunk::Size();

	GetMesh()->Clear();
	Chunk* chunk = (Chunk*)GetEntity();
	if (chunk == nullptr) return;
	STL::Vector<STL::Action<>> pendingActions;

	int overallQuadAmount = 0;
	{
		CHERRYSODA_PROFILE("CalculateChunkMesh");

		for (int i = 0; i < chunkSize; ++i) {
			for (int j = 0; j < chunkSize; ++j) {
				for (int k = 0; k < chunkSize; ++k) {
					if (overallQuadAmount * 4 + 8 > UINT16_MAX) {
						break;
					}
					int index = chunk->GetBlockIndexFast(Math::IVec3(i, j, k));
					Block::Type blockType = chunk->GetBlocks()[index].m_type;
					if (blockType != Block::Type::None) {
						Color color = blockTypeColorMap[(int)blockType];
						auto planeMask = chunk->GetBlockSurroundingFast(index);
						if (planeMask > 0) {
							overallQuadAmount += Math_BitCount(planeMask);
							STL::Add(pendingActions, [planeMask, i, j, k, color, this]() { AddCube(Math::Vec3(i, j, k), 1.f, color, planeMask); });
						}
					}
				}
			}
		}
	}
	{
		CHERRYSODA_PROFILE("UpdateChunkMesh");

		GetMesh()->ReserverAdditional(overallQuadAmount * 4, overallQuadAmount * 6);
		for (auto action : pendingActions) {
			action();
		}
	}
	{
		CHERRYSODA_PROFILE("SubmitBuffer");

		GetMesh()->SubmitBuffer();
	}
}

void ChunkGraphicsComponent::Render()
{
	// TODO: Add virtual BeforeRender to component?
	if (!GetMesh()->IsValid() || GetMesh()->VertexAmount() == 0) return;
	Graphics::SetTextureCube(&GameApp::ms_texCube);
	Graphics::SetTextureCubeIrr(&GameApp::ms_texCubeIrr);

	base::Render();
}

void ChunkGraphicsComponent::AddQuad(const Math::Vec3& pos, float size, const Color& color, const Math::Vec3& normal)
{
	bool positive = (normal == Math_Abs(normal));
	if (normal[1] != 0) positive = !positive;
	const auto pVec = (Vec3_One - Math_Abs(normal)) * size;
	const auto pVecH = normal[0] == 0.f ? Math::Vec3(size, 0.f, 0.f) : Math::Vec3(0.f, size, 0.f);
	const auto pVecV = normal[2] == 0.f ? Math::Vec3(0.f, 0.f, size) : Math::Vec3(0.f, size, 0.f);
	const auto cU32 = color.U32ABGR();
	if (positive) {
		GetMesh()->AddQuad(
			MK_VERT(pos + pVecV, cU32, normal),
			MK_VERT(pos,         cU32, normal),
			MK_VERT(pos + pVec,  cU32, normal),
			MK_VERT(pos + pVecH, cU32, normal)
		);
	}
	else {
		GetMesh()->AddQuad(
			MK_VERT(pos + pVec,  cU32, normal),
			MK_VERT(pos + pVecH, cU32, normal),
			MK_VERT(pos + pVecV, cU32, normal),
			MK_VERT(pos,         cU32, normal)
		);
	}
}

void ChunkGraphicsComponent::AddCube(const Math::Vec3& pos, float size, const Color& color, int planeMask)
{
	if (planeMask & (1 << 0)) AddQuad(pos + Vec3_XUp * size, size, color,  Vec3_XUp);
	if (planeMask & (1 << 1)) AddQuad(pos                  , size, color, -Vec3_XUp);
	if (planeMask & (1 << 2)) AddQuad(pos + Vec3_YUp * size, size, color,  Vec3_YUp);
	if (planeMask & (1 << 3)) AddQuad(pos                  , size, color, -Vec3_YUp);
	if (planeMask & (1 << 4)) AddQuad(pos + Vec3_ZUp * size, size, color,  Vec3_ZUp);
	if (planeMask & (1 << 5)) AddQuad(pos                  , size, color, -Vec3_ZUp);
}
