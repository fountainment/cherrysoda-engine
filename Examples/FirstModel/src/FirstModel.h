#include <CherrySoda/Engine.h>

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Model.h>

namespace firstmodel {

class FirstModel : public cherrysoda::Engine
{
public:
	typedef cherrysoda::Engine base;

	FirstModel();

	void Update() override;
	void Initialize() override;
	void LoadContent() override;

private:
	cherrysoda::Model m_model;
	cherrysoda::Texture2D m_texture;
};

} // namespace firstmodel
