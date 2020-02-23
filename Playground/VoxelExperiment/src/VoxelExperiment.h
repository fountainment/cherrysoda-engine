#include <CherrySoda/Engine.h>

namespace cherrysoda {
	class Effect;
	class TextureCube;
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

	static cherrysoda::TextureCube ms_texCube;
	static cherrysoda::TextureCube ms_texCubeIrr;

	static cherrysoda::Effect ms_voxelShader;
	static cherrysoda::Effect ms_skyboxShader;
};

} // namespace voxelexperiment
