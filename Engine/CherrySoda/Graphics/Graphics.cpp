#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Utility/Log.h>
#include <CherrySoda/Utility/String.h>

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL2/SDL.h>

#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>


using cherrysoda::Graphics;

using cherrysoda::Color;
using cherrysoda::Engine;
using cherrysoda::String;
using cherrysoda::StringUtil;

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;


bgfx::ShaderHandle loadShader(const char* _name) {
	char* data = new char[2048];
	std::ifstream file;
	size_t fileSize = 0;
	file.open(_name, std::ios::binary);
	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		fileSize = file.tellg();
		file.seekg(0, std::ios::beg);
		file.read(data, fileSize);
		file.close();
	}
	const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
	mem->data[mem->size - 1] = '\0';
	bgfx::ShaderHandle handle = bgfx::createShader(mem);
	bgfx::setName(handle, _name);
	return handle;
}


static PosColorVertex s_triVertices[] =
{
	{  0.0f,    0.67f, 0.0f, 0xff0000ff },
	{ -0.577f, -0.33f, 0.0f, 0xff00ff00 },
	{  0.577f, -0.33f, 0.0f, 0xffff0000 }
};

static const uint16_t s_triTriList[] =
{
	0,1,2
};

bgfx::VertexBufferHandle m_vbh;
bgfx::IndexBufferHandle m_ibh;
bgfx::ProgramHandle m_program;

Graphics::Graphics()
{
}

void Graphics::Init()
{
	bgfx::renderFrame();
	bgfx::init();
	bgfx::setDebug(BGFX_DEBUG_TEXT);

	PosColorVertex::init();

	m_vbh = bgfx::createVertexBuffer(
		bgfx::makeRef(s_triVertices, sizeof(s_triVertices))
		, PosColorVertex::ms_layout
	);

	m_ibh = bgfx::createIndexBuffer(
		bgfx::makeRef(s_triTriList, sizeof(s_triTriList))
	);

	bgfx::ShaderHandle vsh = loadShader("vs_simple.bin");
	bgfx::ShaderHandle fsh = loadShader("fs_simple.bin");

	m_program = bgfx::createProgram(vsh, fsh, true);

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
	bgfx::dbgTextPrintf(1, 1, 0x0f, StringUtil::Format("API: %s", bgfx::getRendererName(bgfx::getRendererType())).c_str());
	bgfx::dbgTextPrintf(1, 2, 0x0f, StringUtil::Format("Frame Count: %d", s_frameCount++).c_str());

	const bx::Vec3 at =  { 0.0f, 0.0f,  0.0f };
	const bx::Vec3 eye = { 0.0f, 0.0f, -2.0f };

	float m_width = Engine::GetInstance()->GetWidth();
	float m_height = Engine::GetInstance()->GetHeight();
	{
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.0f, float(m_width) / float(m_height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
		bgfx::setViewTransform(0, view, proj);

		bgfx::setViewRect(0, 0, 0, uint16_t(m_width), uint16_t(m_height));
	}

	bgfx::setVertexBuffer(0, m_vbh);
	bgfx::setIndexBuffer(m_ibh);

	static float zAngle = 0.f;
	float mtx[16];
	bx::mtxRotateXYZ(mtx, 0.f, 0.f, zAngle);
	zAngle += 0.01f;

	bgfx::setTransform(mtx);
	bgfx::setVertexBuffer(0, m_vbh);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(0, m_program);

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