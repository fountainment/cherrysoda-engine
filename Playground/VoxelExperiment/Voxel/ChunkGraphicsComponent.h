#ifndef _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_
#define _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Graphics/GraphicsComponent.h>

#include "Chunk.h"

class ChunkGraphicsComponent : public cherrysoda::GraphicsComponent
{
public:
	typedef cherrysoda::GraphicsComponent base;

	ChunkGraphicsComponent() : base(true) {}

	void Added(cherrysoda::Entity* entity) override;
	void Update() override;
	void Render() override;

private:
	Chunk* m_chunk = nullptr;
};

#endif // _VOXELEXPERIMENT_VOXEL_CHUNKGRAPHICSCOMPONENT_H_