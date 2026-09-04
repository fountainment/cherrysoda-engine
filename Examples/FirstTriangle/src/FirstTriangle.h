#include <CherrySoda/Engine.h>

namespace example {

class FirstTriangle : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstTriangle();

protected:
	void Initialize() override;
};

} // namespace example
