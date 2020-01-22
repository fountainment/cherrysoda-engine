#ifndef _VOXELEXPERIMENT_SKYBOX_H_
#define _VOXELEXPERIMENT_SKYBOX_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>

class Skybox : public cherrysoda::GraphicsComponent
{
public:
	void Render() override;
	void Update() override;
};

#endif // _VOXELEXPERIMENT_SKYBOX_H_