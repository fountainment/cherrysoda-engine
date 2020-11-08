#include <CherrySoda/Engine.h>

namespace noise {

class Noise : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	Noise();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace noise
