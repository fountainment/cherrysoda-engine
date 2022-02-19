#include <CherrySoda/Engine.h>

namespace cherrysodatemplate {

class CherrySodaTemplate : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	CherrySodaTemplate();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;
	void UnloadContent() override;
};

} // namespace cherrysodatemplate
