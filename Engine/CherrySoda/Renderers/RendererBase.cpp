#include <CherrySoda/Renderers/RendererBase.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::RendererBase;

using cherrysoda::Engine;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::Scene;

void RendererBase::RenderPrepare(Scene* scene)
{
	CHERRYSODA_ASSERT(!(RenderPass() == 0 && GetRenderTarget() != nullptr), "Renderpass 0 is preserved to use screen render target.\n");
	Math::IVec2 renderTargetSize = Graphics::GetRenderTargetSize(GetRenderTarget());
	if (!FixedCameraSize()) {
		GetCamera()->SetSize(renderTargetSize);
		if (DoKeepCameraCenterOrigin() && GetCamera()->UseOrthoProjection()) {
			GetCamera()->CenterOrigin();
		}
	}
	Graphics::UseRenderPass(RenderPass())->SetRenderTarget(GetRenderTarget());
	Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, renderTargetSize.x, renderTargetSize.y);
	Graphics::UseRenderPass(RenderPass())->Touch();
	Graphics::SetupEngineUniforms();
}
