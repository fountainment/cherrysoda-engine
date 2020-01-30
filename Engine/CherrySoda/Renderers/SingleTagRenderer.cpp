#include <CherrySoda/Renderers/SingleTagRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/InternalUtilities/EntityList.h>

using cherrysoda::SingleTagRenderer;

using cherrysoda::Graphics;
using cherrysoda::Scene;

void SingleTagRenderer::Render(Scene* scene)
{
	// TODO: RenderState stuffs
	Graphics::SetEffect(&m_effect);
	Graphics::Instance()->SetCamera(GetCamera());
	scene->Entities()->RenderOnly(m_tag);
	// TODO: Add DebugRender
	Graphics::SetEffect(nullptr);
}