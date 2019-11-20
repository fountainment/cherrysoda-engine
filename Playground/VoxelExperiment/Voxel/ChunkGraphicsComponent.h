#ifndef _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

#include "Chunk.h"

namespace crsd = cherrysoda;

class ChunkGraphicsComponent : public cherrysoda::GraphicsComponent
{
public:
	typedef crsd::GraphicsComponent base;
	typedef crsd::Graphics::PosColorNormalVertex VertexType;

	ChunkGraphicsComponent() : base(true) {}

	void Added(cherrysoda::Entity* entity) override;
	void EntityAwake() override;
	void Render() override;
	void Update() override;

	void AddQuad(const crsd::Math::Vec3& pos, float size, const crsd::Color& color, const crsd::Math::Vec3& normal);
	void AddCube(const crsd::Math::Vec3& pos, float size, const crsd::Color& color);

private:
	Chunk* m_chunk = nullptr;
	cherrysoda::Mesh<VertexType> m_mesh;
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_