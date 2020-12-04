#include <CherrySoda/Engine.h>

namespace mathbench {

class MathBench : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	MathBench();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace mathbench
