#ifndef _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_
#define _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Camera.h>

namespace cherrysoda {

class Scene;

class EverythingRenderer : public Renderer
{
public:
	void Render(Scene* scene) override;

	inline Camera* GetCamera() { return &m_camera; }
	inline Effect* GetEffect() { return &m_effect; }

private:
	Camera m_camera;
	Effect m_effect;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_