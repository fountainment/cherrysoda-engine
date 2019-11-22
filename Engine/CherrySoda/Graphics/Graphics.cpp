#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <bgfx/bgfx.h>

#include <sstream>
#include <fstream>

using cherrysoda::Graphics;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Engine;
using cherrysoda::Math;
using cherrysoda::MeshInterface;
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringUtil;

static bgfx::VertexLayout s_posColorLayout;
static bgfx::VertexLayout s_posColorNormalLayout;

void Graphics::PosColorVertex::Init()
{
	s_posColorLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();
}

void Graphics::PosColorNormalVertex::Init()
{
	s_posColorNormalLayout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		.end();
}

bgfx::ShaderHandle loadShader(const char* _name) {
	char data[8192];
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

bgfx::ProgramHandle ms_program;

Graphics::Graphics()
{
}

void Graphics::Init()
{
	bgfx::init();
	// bgfx::setDebug(BGFX_DEBUG_TEXT);

	Graphics::PosColorVertex::Init();
	Graphics::PosColorNormalVertex::Init();

	bgfx::ShaderHandle vsh = loadShader("vs_simple.bin");
	bgfx::ShaderHandle fsh = loadShader("fs_simple.bin");

	ms_program = bgfx::createProgram(vsh, fsh, true);

	ms_uniformCamPos   = bgfx::createUniform("u_camPos", bgfx::UniformType::Vec4).idx;
	ms_uniformMaterial = bgfx::createUniform("u_material", bgfx::UniformType::Vec4, 2).idx;
	ms_uniformLights   = bgfx::createUniform("u_lights", bgfx::UniformType::Vec4, 8).idx;
	ms_uniformTexCube  = bgfx::createUniform("u_texCube", bgfx::UniformType::Sampler).idx;
	ms_uniformParams   = bgfx::createUniform("u_params", bgfx::UniformType::Vec4, 16).idx;

	ms_instance = new Graphics();
}

void Graphics::Terminate()
{
	bgfx::shutdown();
}

void Graphics::RenderFrame()
{
	// static int s_frameCount = 0;

	// bgfx::dbgTextClear();
	// bgfx::dbgTextPrintf(1, 1, 0x0f, StringUtil::Format("API: %s", bgfx::getRendererName(bgfx::getRendererType())).c_str());
	// bgfx::dbgTextPrintf(1, 2, 0x0f, StringUtil::Format("Frame Count: %d", s_frameCount++).c_str());
	// bgfx::dbgTextPrintf(1, 3, 0x0f, StringUtil::Format("Delta Time: %f", Engine::Instance()->DeltaTime()).c_str());

	bgfx::frame();
}

void Graphics::UpdateView()
{
	bgfx::reset(Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight(), m_vsyncEnabled ? BGFX_RESET_VSYNC : BGFX_RESET_NONE);
}

void Graphics::SetClearColor(const Color& color)
{
	bgfx::setViewClear(RenderPass()
		, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
		, color.U32()
		, 1.0f
		, 0
	);
}

void Graphics::Touch()
{
	bgfx::touch(RenderPass());
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
	bgfx::setViewRect(RenderPass(), x, y, w, h);
}

void Graphics::SetCamera(Camera* camera)
{
	camera->UpdateMatrices();
	bgfx::setViewTransform(RenderPass(), &camera->m_viewMatrix, &camera->m_projMatrix);
}

void Graphics::SetTransformMatrix(const Math::Mat4& transformMatrix)
{
	bgfx::setTransform(&transformMatrix);
}

void Graphics::SetMesh(MeshInterface* mesh)
{
	SetVertexBuffer(mesh->GetVertexBuffer());
	SetIndexBuffer(mesh->GetIndexBuffer());
}

void Graphics::SetVertexBuffer(Graphics::VertexBufferHandle vertexBuffer)
{
	bgfx::setVertexBuffer(RenderPass(), bgfx::VertexBufferHandle{vertexBuffer});
}

void Graphics::SetIndexBuffer(Graphics::IndexBufferHandle indexBuffer)
{
	bgfx::setIndexBuffer(bgfx::IndexBufferHandle{indexBuffer});
}

void Graphics::Submit()
{
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(RenderPass(), ms_program);
}

Graphics::VertexBufferHandle Graphics::CreateVertexBuffer(STL::Vector<Graphics::PosColorVertex>& vertices)
{
	return bgfx::createVertexBuffer(
		bgfx::makeRef(STL::Data(vertices), STL::ByteSize(vertices)),
		s_posColorLayout
	).idx;
}

Graphics::VertexBufferHandle Graphics::CreateVertexBuffer(STL::Vector<Graphics::PosColorNormalVertex>& vertices)
{
	return bgfx::createVertexBuffer(
		bgfx::makeRef(STL::Data(vertices), STL::ByteSize(vertices)),
		s_posColorNormalLayout
	).idx;
}

Graphics::IndexBufferHandle Graphics::CreateIndexBuffer(STL::Vector<cherrysoda::type::UInt16>& indices)
{
	return bgfx::createIndexBuffer(
		bgfx::makeRef(STL::Data(indices), STL::ByteSize(indices))
	).idx;
}

void Graphics::SetUniformCamPos(const Math::Vec3& camPos)
{
	Math::Vec4 camPosVec4 = Math::Vec4(camPos, 1.0f);
	bgfx::setUniform({ ms_uniformCamPos }, &camPosVec4);
}

void Graphics::SetUniformMaterial(const Math::Vec3& albedo, float metallic, float roughness, float ao)
{
	Math::Vec4 materialVec4[] = { Math::Vec4(albedo, 0.0f), Math::Vec4(metallic, roughness, ao, 0.0f) };
	bgfx::setUniform({ ms_uniformMaterial }, materialVec4, 2U);
}

void Graphics::SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor)
{
	static Math::Vec4 lightVec4[8];
	lightVec4[index * 2] = Math::Vec4(lightPos, 1.0f);
	lightVec4[index * 2 + 1] = Math::Vec4(lightColor, 1.0f);
	bgfx::setUniform ({ ms_uniformLights }, lightVec4, 8U);
}

Graphics::UniformHandle Graphics::ms_uniformCamPos   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformLights   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformMaterial = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformParams   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformTexCube  = Graphics::InvalidHandle;
Graphics* Graphics::ms_instance = nullptr;