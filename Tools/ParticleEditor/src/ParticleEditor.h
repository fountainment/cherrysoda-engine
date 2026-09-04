#include <CherrySoda/Engine.h>

namespace particleeditor {

class ParticleEditor : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	ParticleEditor();

protected:
	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace particleeditor
