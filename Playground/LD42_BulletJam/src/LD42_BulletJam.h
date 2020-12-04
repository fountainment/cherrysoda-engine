#include <CherrySoda/Engine.h>

namespace cherrysoda {
class Atlas;
} // namespace cherrysoda

namespace ld42_bulletjam {

class LD42_BulletJam : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	LD42_BulletJam();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;

	static cherrysoda::Atlas* GetAtlas();
};

} // namespace ld42_bulletjam
