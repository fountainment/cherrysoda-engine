#ifndef _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_
#define _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_

#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Camera.h>

namespace cherrysoda {

class Scene;

class EverythingRenderer : public Renderer
{
public:
	void Render(Scene* scene) override;

	inline Camera* GetCamera() { return &m_camera; }

private:
	Camera m_camera;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_