#ifndef _VOXELEXPERIMENT_SKYBOX_H_
#define _VOXELEXPERIMENT_SKYBOX_H_

#include <CherrySoda/Components/Graphics/GraphicsComponent.h>

class Skybox : public cherrysoda::GraphicsComponent
{
public:
	typedef cherrysoda::GraphicsComponent base;

	Skybox() : base(true) {}

	void Render() override;
};

#endif // _VOXELEXPERIMENT_SKYBOX_H_
