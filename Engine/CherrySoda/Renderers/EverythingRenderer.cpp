#include <CherrySoda/Renderers/EverythingRenderer.h>

#include <CherrySoda/Scene.h>
#include <CherrySoda/InternalUtilities/EntityList.h>

using cherrysoda::EverythingRenderer;

using cherrysoda::Scene;

void EverythingRenderer::Render(Scene* scene)
{
	// TODO: Camera, RenderState, Shader stuffs
	scene->Entities()->Render();
}