#include <CherrySoda/Renderers/EverythingRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Util/Draw.h>

namespace cherrysoda {

void EverythingRenderer::Render(Scene* scene)
{
	Graphics::BeginRenderPass(RenderPass());
	Graphics::SetEffect(GetEffect());
	Graphics::UseCurrentRenderPass()->SetCamera(GetCamera());
	Draw::GetSpriteBatch()->Begin();
	scene->Entities()->Render();
	if (Engine::Instance()->ConsoleOpened())
		scene->Entities()->DebugRender(GetCamera());
	Draw::GetSpriteBatch()->End();
	Graphics::SetEffect(nullptr);
	Graphics::EndRenderPass(RenderPass());
}

} // namespace cherrysoda
