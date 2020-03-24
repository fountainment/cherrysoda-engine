#include <CherrySoda/Renderers/EverythingRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Util/Draw.h>

using cherrysoda::EverythingRenderer;

using cherrysoda::Draw;
using cherrysoda::Graphics;
using cherrysoda::Scene;

void EverythingRenderer::Render(Scene* scene)
{
	// TODO: RenderState stuffs
	Graphics::BeginRenderPass(RenderPass());
	Graphics::SetEffect(GetEffect());
	Graphics::UseCurrentRenderPass()->SetCamera(GetCamera());
	Draw::GetSpriteBatch()->Begin();
	scene->Entities()->Render();
	// TODO: Add DebugRender
	Draw::GetSpriteBatch()->End();
	Graphics::SetEffect(nullptr);
	Graphics::EndRenderPass(RenderPass());
	Graphics::Discard();
}
