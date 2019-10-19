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
	bgfx::renderFrame();
	bgfx::init();
	bgfx::setDebug(BGFX_DEBUG_TEXT);

	ms_instance = new Graphics();
}

void Graphics::Terminate()
{
	bgfx::shutdown();
}

void Graphics::RenderFrame()
{
	static int s_frameCount = 0;

	Graphics::SetViewport(0, 0, Engine::GetInstance()->GetWidth(), Engine::GetInstance()->GetHeight());
	bgfx::touch(0);
	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(0, 1, 0x0f, "Hello, world!");
	bgfx::dbgTextPrintf(0, 2, 0x0f, StringUtil::Format("API: %s", bgfx::getRendererName(bgfx::getRendererType())).c_str());
	bgfx::dbgTextPrintf(0, 3, 0x0f, StringUtil::Format("Frame: %d", s_frameCount++).c_str());
	bgfx::frame();
}

void Graphics::UpdateView()
{
	bgfx::reset(Engine::GetInstance()->GetWidth(), Engine::GetInstance()->GetHeight(), m_vsyncEnabled ? BGFX_RESET_VSYNC : BGFX_RESET_NONE);
}

void Graphics::SetClearColor(const Color& color)
{
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, color.U32()
		, 1.0f
		, 0
	);
}

void Graphics::SetViewport(int x, int y, int w, int h)
{
	bgfx::setViewRect(0, x, y, w, h);
}

Graphics* Graphics::ms_instance = nullptr;