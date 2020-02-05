#include "Skybox.h"

#include <CherrySoda/Graphics/Graphics.h>

using cherrysoda::Graphics;

void Skybox::Render()
{
	Graphics::Instance()->ScreenSpaceQuad(1.f, 1.f);
	Graphics::SetStateNoDepth();
	Graphics::Instance()->Submit();
}