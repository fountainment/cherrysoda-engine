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
	Graphics::SetEffect(&m_effect);
	Graphics::Instance()->SetCamera(GetCamera());
	scene->Entities()->Render();
	Graphics::SetEffect(nullptr);
}