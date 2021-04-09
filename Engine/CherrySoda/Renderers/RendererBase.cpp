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

void RendererBase::BeforeRender(Scene* scene)
{
	Math::IVec2 renderTargetSize = Graphics::GetRenderTargetSize(GetRenderTarget());
	if (!FixedCameraSize()) {
		GetCamera()->SetSize(renderTargetSize);
		if (KeepCameraCenterOrigin()) {
			GetCamera()->CenterOrigin();
		}
	}
	Graphics::UseRenderPass(RenderPass())->SetRenderTarget(GetRenderTarget());
	Graphics::UseRenderPass(RenderPass())->SetViewport(0, 0, renderTargetSize.x, renderTargetSize.y);
	Graphics::UseRenderPass(RenderPass())->Touch();
}
