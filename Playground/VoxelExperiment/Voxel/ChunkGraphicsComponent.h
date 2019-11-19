#ifndef _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Graphics/Graphics.h>

#include "Chunk.h"

class ChunkGraphicsComponent : public cherrysoda::GraphicsComponent
{
public:
	typedef cherrysoda::GraphicsComponent base;

	ChunkGraphicsComponent() : base(true) {}

	void Added(cherrysoda::Entity* entity) override;
	void EntityAwake() override;
	void Render() override;

	void Update() override;

private:
	Chunk* m_chunk = nullptr;
	cherrysoda::Mesh<cherrysoda::Graphics::PosColorNormalVertex> m_mesh;
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_