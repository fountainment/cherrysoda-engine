#include <CherrySoda/Engine.h>

namespace firstparticle {

class FirstParticle : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstParticle();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace firstparticle
