#include <CherrySoda/Engine.h>

namespace easefunction {

class EaseFunction : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	EaseFunction();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace easefunction
