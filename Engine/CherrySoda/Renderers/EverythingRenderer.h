#ifndef _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_
#define _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_

#include <CherrySoda/Renderers/Renderer.h>

namespace cherrysoda {

class Scene;

class EverythingRenderer : public Renderer
{
public:
	void Render(Scene* scene) override;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_