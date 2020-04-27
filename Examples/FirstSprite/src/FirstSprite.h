#include <CherrySoda/Engine.h>

namespace example {

class FirstSprite : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstSprite();

	void Initialize() override;
};

} // namespace example
