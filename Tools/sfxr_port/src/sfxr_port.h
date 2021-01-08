#include <CherrySoda/Engine.h>

namespace sfxr_port {

class Sfxr_port : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	Sfxr_port();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace sfxr_port
