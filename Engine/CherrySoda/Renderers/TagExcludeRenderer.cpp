#include <CherrySoda/Renderers/TagExcludeRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/InternalUtilities/EntityList.h>
#include <CherrySoda/Util/Draw.h>

using cherrysoda::TagExcludeRenderer;

using cherrysoda::Draw;
using cherrysoda::Graphics;
using cherrysoda::Scene;

void TagExcludeRenderer::Render(Scene* scene)
{
	Graphics::BeginRenderPass(RenderPass());
	Graphics::SetEffect(GetEffect());
	Graphics::UseCurrentRenderPass()->SetCamera(GetCamera());
	Draw::GetSpriteBatch()->Begin();
	for (auto entity : *(scene->Entities())) {
		if (entity->Visible() && (entity->Tag() & m_excludeTag) == 0) {
			entity->Render();
		}
	}
	if (Engine::Instance()->ConsoleOpened()) {
		for (auto entity : *(scene->Entities())) {
			if ((entity->Tag() & m_excludeTag) == 0) {
				entity->DebugRender(GetCamera());
			}
		}
	}
	Draw::GetSpriteBatch()->End();
	Graphics::SetEffect(nullptr);
	Graphics::EndRenderPass(RenderPass());
}
