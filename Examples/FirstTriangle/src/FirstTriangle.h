#include <CherrySoda/Engine.h>

namespace cherrysoda {
	class Effect;
} // namespace cherrysoda

namespace example {

class FirstTriangle : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstTriangle();

	void Initialize() override;

	static cherrysoda::Effect ms_shader;
};

} // namespace voxelexperiment