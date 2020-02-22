#include <CherrySoda/Engine.h>

namespace example {

class FirstTriangle : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstTriangle();

	void Initialize() override;
};

} // namespace voxelexperiment