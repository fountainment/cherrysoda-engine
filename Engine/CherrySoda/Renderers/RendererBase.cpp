#include <CherrySoda/Renderers/RendererBase.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

void RendererBase::RenderPrepare(Scene* /*scene*/)
{
	CHERRYSODA_ASSERT(!(RenderPass() == 0 && GetRenderTarget() != nullptr),
					  "Renderpass 0 is preserved to use screen render target.\n");
	Math::IVec2 renderTargetSize = Graphics::GetRenderTargetSize(GetRenderTarget());
	if (!FixedCameraSize()) {
		GetCamera()->SetSize(renderTargetSize);
		if (DoKeepCameraCenterOrigin() && GetCamera()->UseOrthoProjection()) {
			GetCamera()->CenterOrigin();
		}
	}
	// the screen render pass covers the physical window while the camera keeps
	// the logical view size, scaling content up on high-DPI displays
	Math::IVec2 viewportSize = GetRenderTarget() != nullptr ? renderTargetSize : Engine::Instance()->GetWindowSize();
	Graphics::UseRenderPass(RenderPass())->SetRenderTarget(GetRenderTarget());
	Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, viewportSize.x, viewportSize.y);
	Graphics::UseRenderPass(RenderPass())->Touch();
	Graphics::SetupEngineUniforms();
}

} // namespace cherrysoda
