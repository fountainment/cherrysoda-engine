#ifndef _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Graphics/MeshGraphicsComponent.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

#include "Chunk.h"

namespace crsd = cherrysoda;

class ChunkGraphicsComponent : public crsd::MeshGraphicsComponent<crsd::Graphics::PosColorNormalVertex>
{
public:
	typedef crsd::MeshGraphicsComponent<VertexType> base;

	ChunkGraphicsComponent() : base(true) {}

	void EntityAwake() override;
	void Render() override;

	void AddQuad(const crsd::Math::Vec3& pos, float size, const crsd::Color& color, const crsd::Math::Vec3& normal);
	void AddCube(const crsd::Math::Vec3& pos, float size, const crsd::Color& color, int planeMask = 0xffffffff);

	void RebuildMesh();
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_
