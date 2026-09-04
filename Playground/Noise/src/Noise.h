#include <CherrySoda/Engine.h>

namespace noise {

class Noise : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	Noise();

protected:
	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace noise
