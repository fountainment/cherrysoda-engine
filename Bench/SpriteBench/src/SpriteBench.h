#include <CherrySoda/Engine.h>

namespace spritebench {

class SpriteBench : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	SpriteBench();

protected:
	void Update() override;
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;
};

} // namespace spritebench
