#ifndef _CHERRYSODA_RENDERERS_RENDERERBASE_H_
#define _CHERRYSODA_RENDERERS_RENDERERBASE_H_

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/RenderTarget.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/NumType.h>

namespace cherrysoda {

class Scene;

class RendererBase : public Renderer
{
public:
	Camera* GetCamera() override { return &m_camera; }
	RenderTarget2D* GetRenderTarget() override { return m_renderTarget; }

	inline Effect* GetEffect() { return &m_effect; }
	inline void SetEffect(const Effect& effect) { *GetEffect() = effect; }
	inline void SetRenderTarget(RenderTarget2D* renderTarget) { m_renderTarget = renderTarget; }

	void RenderPrepare(Scene* scene) override;

	inline type::UInt16 RenderPass() { return m_renderPass; };
	inline void RenderPass(type::UInt16 renderPass) { m_renderPass = renderPass; }

	inline void FixedCameraSize(bool fixedCameraSize) { m_fixedCameraSize = fixedCameraSize; }
	inline bool FixedCameraSize() { return m_fixedCameraSize; }

	inline void KeepCameraCenterOrigin(bool keepCameraCenterOrigin) { m_keepCameraCenterOrigin = keepCameraCenterOrigin; }
	inline bool DoKeepCameraCenterOrigin() { return m_keepCameraCenterOrigin; }

protected:
	Camera m_camera;
	Effect m_effect;
	RenderTarget2D* m_renderTarget = nullptr;
	type::UInt16 m_renderPass = 0;

	bool m_fixedCameraSize = false;
	bool m_keepCameraCenterOrigin = false;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_RENDERERS_RENDERERBASE_H_
