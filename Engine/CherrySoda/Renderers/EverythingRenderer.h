#ifndef _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_
#define _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class Scene;

class EverythingRenderer : public Renderer
{
public:
	void Render(Scene* scene) override;

	inline Camera* GetCamera() override { return &m_camera; }
	inline Effect* GetEffect() { return &m_effect; }

	inline void SetEffect(const Effect& effect) { *GetEffect() = effect; }

	inline type::UInt16 RenderPass() { return m_renderPass; };
	inline void RenderPass(type::UInt16 renderPass) { m_renderPass = renderPass; }

private:
	Camera m_camera;
	Effect m_effect;
	type::UInt16 m_renderPass = 0;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_EVERYTHINGRENDERER_H_
