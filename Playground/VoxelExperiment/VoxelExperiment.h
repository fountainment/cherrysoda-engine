#include <CherrySoda/Engine.h>

namespace cherrysoda {
	class Effect;
	class Texture;
} // namespace cherrysoda

namespace voxelexperiment {

class VoxelExperiment : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	VoxelExperiment();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;

	static cherrysoda::Texture ms_texCube;
	static cherrysoda::Texture ms_texCubeIrr;

	static cherrysoda::Effect ms_voxelShader;
	static cherrysoda::Effect ms_skyboxShader;
};

}

using GameApp = voxelexperiment::VoxelExperiment;