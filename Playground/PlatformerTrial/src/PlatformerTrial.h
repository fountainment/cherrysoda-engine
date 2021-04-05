#include <CherrySoda/Engine.h>

namespace platformertrial {

class PlatformerTrial : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	PlatformerTrial();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace platformertrial
