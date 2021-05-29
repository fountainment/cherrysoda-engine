#include <CherrySoda/Engine.h>

namespace firstfont {

class FirstFont : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstFont();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
};

} // namespace firstfont
