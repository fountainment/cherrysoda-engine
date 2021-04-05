#include <CherrySoda/Renderers/RendererBase.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::RendererBase;

using cherrysoda::Engine;
using cherrysoda::Graphics;
using cherrysoda::Math;

void RendererBase::BeforeRender(Scene* scene)
{
	Graphics::UseRenderPass(RenderPass())->SetRenderTarget(GetRenderTarget());
	Math::IVec2 renderTargetSize = Graphics::GetRenderTargetSize(GetRenderTarget());
	Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, renderTargetSize.x, renderTargetSize.y);
	if (!FixedCameraSize()) {
		GetCamera()->SetSize(renderTargetSize);
		if (KeepCameraCenterOrigin()) {
			GetCamera()->CenterOrigin();
		}
	}
}
