#include <CherrySoda/Engine.h>

namespace platformertrial {

class PlatformerTrial : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	PlatformerTrial();

protected:
	void Update() override;
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;
};

} // namespace platformertrial
