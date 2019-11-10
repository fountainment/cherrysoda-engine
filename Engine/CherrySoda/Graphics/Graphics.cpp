#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>

#include <bx/bx.h>
#include <bx/math.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm/gtc/matrix_transform.hpp>

#include <cstdio>
#include <string>
#include <sstream>
#include <fstream>


using cherrysoda::Graphics;

using cherrysoda::Camera;
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
	char data[2048];
	std::ifstream file;
	size_t fileSize = 0;
	file.open(_name, std::ios::binary);
	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		fileSize = static_cast<size_t>(file.tellg());
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
	{  0.0f,   0.67f, 0.0f, 0xff0000ff },
	{ -0.58f, -0.33f, 0.0f, 0xff00ff00 },
	{  0.58f, -0.33f, 0.0f, 0xffff0000 }
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
	bgfx::init();
	//bgfx::setDebug(BGFX_DEBUG_TEXT);

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

	//bgfx::dbgTextClear();
	//bgfx::dbgTextPrintf(1, 1, 0x0f, StringUtil::Format("API: %s", bgfx::getRendererName(bgfx::getRendererType())).c_str());
	//bgfx::dbgTextPrintf(1, 2, 0x0f, StringUtil::Format("Frame Count: %d", s_frameCount++).c_str());
	//bgfx::dbgTextPrintf(1, 3, 0x0f, StringUtil::Format("Delta Time: %f", Engine::Instance()->DeltaTime()).c_str());

	const glm::vec3 eye    = { 0.0f, 0.0f, -2.0f };
	const glm::vec3 center = { 0.0f, 0.0f,  0.0f };
	const glm::vec3 up     = { 0.0f, 0.1f,  0.0f };

	uint16_t width  = Engine::Instance()->GetWidth();
	uint16_t height = Engine::Instance()->GetHeight();
	float aspect = Engine::Instance()->GetAspectRatio();
	glm::mat4 viewMtx = glm::lookAt(eye, center, up);
	glm::mat4 projMtx = glm::ortho(-1.f * aspect, 1.0f * aspect, -1.f, 1.0f, -100.0f, 100.0f);
	bgfx::setViewTransform(0, &viewMtx, &projMtx);

	static float zAngle = 0.f;
	glm::mat4 transMtx = glm::rotate(glm::identity<glm::mat4>(), zAngle, glm::vec3(0.f, 0.f, 1.0f));
	float deltaTime = Engine::Instance()->DeltaTime();
	float maxDeltaTime = 1.0f / 30.0f;
	deltaTime = deltaTime > maxDeltaTime ? maxDeltaTime : deltaTime;
	zAngle += 1.0f * deltaTime;

	bgfx::setTransform(&transMtx);
	bgfx::setVertexBuffer(0, m_vbh);
	bgfx::setIndexBuffer(m_ibh);
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(0, m_program);

	bgfx::frame();
}

void Graphics::UpdateView()
{
	bgfx::reset(Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight(), m_vsyncEnabled ? BGFX_RESET_VSYNC : BGFX_RESET_NONE);
}

void Graphics::SetClearColor(const Color& color)
{
	bgfx::setViewClear(m_viewId
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, color.U32()
		, 1.0f
		, 0
	);
}

void Graphics::Touch()
{
	bgfx::touch(m_viewId);
}

void Graphics::SetVsyncEnabled(bool vsyncEnabled)
{
	m_vsyncEnabled = vsyncEnabled;
	if (Engine::Instance()->Initialized()) {
		UpdateView();
	}
}

void Graphics::SetViewport(int x, int y, int w, int h)
{
	bgfx::setViewRect(m_viewId, x, y, w, h);
}

void Graphics::SetCamera(Camera* camera)
{
	bgfx::setViewTransform(m_viewId, &camera->m_viewMatrix, &camera->m_projMatrix);
}

Graphics* Graphics::ms_instance = nullptr;
