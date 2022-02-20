#include <CherrySoda/Engine.h>

namespace cherrysoda {

class Atlas;

} // namespace cherrysoda

namespace tilebasedplatformertrial {

class TileBasedPlatformerTrial : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	TileBasedPlatformerTrial();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;

	static cherrysoda::Atlas* GetAtlas() { return ms_atlas; }

private:
	static cherrysoda::Atlas* ms_atlas;
};

} // namespace tilebasedplatformertrial
