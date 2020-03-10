#include "Skybox.h"

#include "Program.h"

#include <CherrySoda/Graphics/Graphics.h>

using cherrysoda::Graphics;

void Skybox::Render()
{
	Graphics::SetTextureCube(&GameApp::ms_texCube);
	Graphics::ScreenSpaceQuad(1.f, 1.f);
	Graphics::SetStateNoDepth();
	Graphics::SubmitOnCurrentRenderPass();
}
