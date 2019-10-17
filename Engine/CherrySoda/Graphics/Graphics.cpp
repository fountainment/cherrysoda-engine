#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Utility/Log.h>
#include <CherrySoda/Utility/String.h>

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL2/SDL.h>

#include <cstdio>


using cherrysoda::Graphics;

using cherrysoda::Color;
using cherrysoda::Engine;
using cherrysoda::String;
using cherrysoda::StringUtil;

Graphics::Graphics()
{
}

void Graphics::Init()
{
	bgfx::init();
	bgfx::shutdown();
	Graphics::LoadGraphicsAPI();
	Graphics::SetViewport(0, 0, Engine::GetInstance()->GetWidth(), Engine::GetInstance()->GetHeight());

	ms_instance = new Graphics();
}

void Graphics::LoadGraphicsAPI()
{
	//CHERRYSODA_DEBUG(StringUtil::Format("OpenGL Version: %s\n", glGetString(GL_VERSION)));
	//CHERRYSODA_DEBUG(StringUtil::Format("GLSL Version:   %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)));
}

void Graphics::SetClearColor(const Color& color)
{
	//glClearColor(color.R(), color.G(), color.B(), color.A());
}

void Graphics::SetViewport(int x, int y, int w, int h)
{
	//glViewport(x, y, w, h);
}

void Graphics::ClearColorAndDepth()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Graphics* Graphics::ms_instance = nullptr;