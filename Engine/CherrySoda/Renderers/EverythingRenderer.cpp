#include <CherrySoda/Renderers/EverythingRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/InternalUtilities/EntityList.h>

using cherrysoda::EverythingRenderer;

using cherrysoda::Graphics;
using cherrysoda::Scene;

void EverythingRenderer::Render(Scene* scene)
{
	// TODO: RenderState stuffs
	Graphics::BeginRenderPass(RenderPass());
	Graphics::SetEffect(GetEffect());
	Graphics::UseCurrentRenderPass()->SetCamera(GetCamera());
	scene->Entities()->Render();
	// TODO: Add DebugRender
	Graphics::SetEffect(nullptr);
	Graphics::EndRenderPass(RenderPass());
}
