#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Graphics/RenderTarget.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Renderers/Renderer.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <bgfx/bgfx.h>
#include <bgfx/embedded_shader.h>
#include <bx/bx.h>
#include <bx/debug.h>
#include <bx/file.h>
#include <bx/allocator.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>

#include <sstream>
#include <fstream>

#include "embedded_shaders.h"

using cherrysoda::Graphics;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Math;
using cherrysoda::MeshInterface;
using cherrysoda::RenderTarget2D;
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::StringUtil;
using cherrysoda::Texture;
using cherrysoda::Texture2D;
using cherrysoda::TextureCube;

namespace type = cherrysoda::type;

#define DBG_STRINGIZE(_x) DBG_STRINGIZE_(_x)
#define DBG_STRINGIZE_(_x) #_x
#define DBG_FILE_LINE_LITERAL "" __FILE__ "(" DBG_STRINGIZE(__LINE__) "): "
#define DBG(_format, ...) bx::debugPrintf(DBG_FILE_LINE_LITERAL "" _format "\n", ##__VA_ARGS__)

static uint64_t s_defaultTextureSamplerFlags = 0
	| BGFX_SAMPLER_U_CLAMP
	| BGFX_SAMPLER_V_CLAMP;

static constexpr uint64_t s_linearTextureSamplerFlags = 0
	| BGFX_SAMPLER_U_CLAMP
	| BGFX_SAMPLER_V_CLAMP;

static constexpr uint64_t s_pointTextureSamplerFlags = 0
	| BGFX_SAMPLER_MIN_POINT
	| BGFX_SAMPLER_MAG_POINT
	| BGFX_SAMPLER_MIP_POINT
	| BGFX_SAMPLER_U_CLAMP
	| BGFX_SAMPLER_V_CLAMP;

static inline uint64_t GetTextureSamplerFlags()
{
	return s_defaultTextureSamplerFlags;
}

static bgfx::TextureFormat::Enum s_defaultDepthFormat = bgfx::TextureFormat::Enum::D24;

static STL::HashMap<StringID,Effect> s_embeddedEffects;

class PosColorDefinition
{
public:
	typedef Graphics::PosColorVertex VertexType;
	static bgfx::VertexLayout s_layout;
};

class PosColorNormalDefinition
{
public:
	typedef Graphics::PosColorNormalVertex VertexType;
	static bgfx::VertexLayout s_layout;
};

class PosColorTexCoord0Definition
{
public:
	typedef Graphics::PosColorTexCoord0Vertex VertexType;
	static bgfx::VertexLayout s_layout;
};

class PosNormalTexCoord0Definition
{
public:
	typedef Graphics::PosNormalTexCoord0Vertex VertexType;
	static bgfx::VertexLayout s_layout;
};

class PosColorNormalTexCoord0Definition
{
public:
	typedef Graphics::PosColorNormalTexCoord0Vertex VertexType;
	static bgfx::VertexLayout s_layout;
};

class ImGuiVertexDefinition
{
public:
	typedef Graphics::ImGuiVertex VertexType;
	static bgfx::VertexLayout s_layout;
};

bgfx::VertexLayout PosColorDefinition::s_layout;
bgfx::VertexLayout PosColorNormalDefinition::s_layout;
bgfx::VertexLayout PosColorTexCoord0Definition::s_layout;
bgfx::VertexLayout PosNormalTexCoord0Definition::s_layout;
bgfx::VertexLayout PosColorNormalTexCoord0Definition::s_layout;
bgfx::VertexLayout ImGuiVertexDefinition::s_layout;

STL::Vector<bgfx::TransientIndexBuffer> s_transientIndexBufferStack;
STL::Vector<bgfx::TransientVertexBuffer> s_transientVertexBufferStack;

namespace entry {

static bgfx::PlatformData s_platformData;

static bx::DefaultAllocator s_allocator;
static bx::FileReaderI* s_fileReader;
static bx::FileWriterI* s_fileWriter;

bx::AllocatorI* g_allocator = &s_allocator;

typedef bx::StringT<&g_allocator> bxString;

static bxString s_currentDir;

class FileReader : public bx::FileReader
{
	typedef bx::FileReader super;

public:
	virtual bool open(const bx::FilePath& _filePath, bx::Error* _err) override
	{
		bxString filePath(s_currentDir);
		filePath.append(_filePath);
		return super::open(filePath.getPtr(), _err);
	}
};

class FileWriter : public bx::FileWriter
{
	typedef bx::FileWriter super;

public:
	virtual bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
	{
		bxString filePath(s_currentDir);
		filePath.append(_filePath);
		return super::open(filePath.getPtr(), _append, _err);
	}
};

void setCurrentDir(const char* _dir)
{
	s_currentDir.set(_dir);
}

void init()
{
	s_fileReader = BX_NEW(g_allocator, FileReader);
	s_fileWriter = BX_NEW(g_allocator, FileWriter);
}

void termiate()
{
	BX_DELETE(g_allocator, s_fileReader);
	s_fileReader = nullptr;
	BX_DELETE(g_allocator, s_fileWriter);
	s_fileWriter = nullptr;
}

bx::FileReaderI* getFileReader()
{
	return s_fileReader;
}

bx::FileWriterI* getFileWriter()
{
	return s_fileWriter;
}

bx::AllocatorI* getAllocator()
{
	return g_allocator;
}

} // namespace entry

void Graphics::PosColorVertex::Init()
{
	PosColorDefinition::s_layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
		.end();
}

void Graphics::PosColorNormalVertex::Init()
{
	PosColorNormalDefinition::s_layout
		.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0,   4, bgfx::AttribType::Uint8, true)
		.add(bgfx::Attrib::Normal,   4, bgfx::AttribType::Uint8, true, true)
		.end();
}

void Graphics::PosColorTexCoord0Vertex::Init()
{
	PosColorTexCoord0Definition::s_layout
		.begin()
		.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();
}

void Graphics::PosNormalTexCoord0Vertex::Init()
{
	PosNormalTexCoord0Definition::s_layout
		.begin()
		.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal,    4, bgfx::AttribType::Uint8, true, true)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();
}

void Graphics::PosColorNormalTexCoord0Vertex::Init()
{
	PosColorNormalTexCoord0Definition::s_layout
		.begin()
		.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
		.add(bgfx::Attrib::Normal,    4, bgfx::AttribType::Uint8, true, true)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.end();
}

void Graphics::ImGuiVertex::Init()
{
	ImGuiVertexDefinition::s_layout
		.begin()
		.add(bgfx::Attrib::Position,  2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0,    4, bgfx::AttribType::Uint8, true)
		.end();
}

bgfx::ShaderHandle loadShader(const String& name) {
	char* data = nullptr;
	std::ifstream file;
	size_t fileSize = 0;
	file.open(name.c_str(), std::ios::binary);
	if (file.is_open()) {
		file.seekg(0, std::ios::end);
		fileSize = static_cast<size_t>(file.tellg());
		data = new char[fileSize + 1];
		file.seekg(0, std::ios::beg);
		file.read(data, fileSize);
		file.close();

		auto dataRelease = [](void* ptr, void* userdata)
		{
			char* data = (char*)userdata;
			delete [] data;
		};
		const bgfx::Memory* mem = bgfx::makeRef(data, static_cast<uint32_t>(fileSize) + 1, dataRelease, data);
		mem->data[mem->size - 1] = '\0';
		bgfx::ShaderHandle handle = bgfx::createShader(mem);
		bgfx::setName(handle, name.c_str());
		return handle;
	}
	return { bgfx::kInvalidHandle };
}

bgfx::ShaderHandle loadEmbeddedShader(const String& name)
{
	bgfx::RendererType::Enum type = bgfx::getRendererType();
	bgfx::ShaderHandle handle = bgfx::createEmbeddedShader(s_embeddedShaders, type, name.c_str());
	bgfx::setName(handle, name.c_str());
	return handle;
}

bgfx::ProgramHandle loadProgram(const String& vs, const String& fs)
{
	String shaderPath = "???";
	switch (bgfx::getRendererType())
	{
	case bgfx::RendererType::Noop:
	case bgfx::RendererType::Direct3D9:  shaderPath = "shaders/dx9/";   break;
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12: shaderPath = "shaders/dx11/";  break;
	case bgfx::RendererType::Agc:
	case bgfx::RendererType::Gnm:        shaderPath = "shaders/pssl/";  break;
	case bgfx::RendererType::Metal:      shaderPath = "shaders/metal/"; break;
	case bgfx::RendererType::Nvn:        shaderPath = "shaders/nvn/";   break;
	case bgfx::RendererType::OpenGL:     shaderPath = "shaders/glsl/";  break;
#if defined(__ANDROID__)
	case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl_a/";break;
#else
	case bgfx::RendererType::OpenGLES:   shaderPath = "shaders/essl/";  break;
#endif
	case bgfx::RendererType::Vulkan:     shaderPath = "shaders/spirv/"; break;
	case bgfx::RendererType::WebGPU:     shaderPath = "shaders/spirv/"; break;

	case bgfx::RendererType::Count:
		CHERRYSODA_ASSERT(false, "You should not be here!");
		break;
	}
	String path_prefix = "assets/" + shaderPath;
	return bgfx::createProgram(loadShader(path_prefix + vs + ".bin"), loadShader(path_prefix + fs + ".bin"), true);
}

bgfx::ProgramHandle loadEmbeddedProgram(const String& vs, const String& fs)
{
	return bgfx::createProgram(loadEmbeddedShader(vs), loadEmbeddedShader(fs), true);
}

void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
{
	if (bx::open(_reader, _filePath) )
	{
		uint32_t size = (uint32_t)bx::getSize(_reader);
		void* data = BX_ALLOC(_allocator, size);
		bx::read(_reader, data, size, bx::ErrorAssert{});
		bx::close(_reader);
		if (NULL != _size)
		{
			*_size = size;
		}
		return data;
	}
	else
	{
		DBG("Failed to open: %s.", _filePath);
	}

	if (NULL != _size)
	{
		*_size = 0;
	}

	return NULL;
}

void* load(const char* _filePath, uint32_t* _size)
{
	return load(entry::getFileReader(), entry::getAllocator(), _filePath, _size);
}

void unload(void* _ptr)
{
	BX_FREE(entry::getAllocator(), _ptr);
}

static void imageReleaseCb(void* _ptr, void* _userData)
{
	BX_UNUSED(_ptr);
	bimg::ImageContainer* imageContainer = (bimg::ImageContainer*)_userData;
	bimg::imageFree(imageContainer);
}

bgfx::TextureHandle loadTexture(bx::FileReaderI* _reader, const char* _filePath, uint64_t _flags, uint8_t _skip, bgfx::TextureInfo* _info, bimg::Orientation::Enum* _orientation)
{
	BX_UNUSED(_skip);
	bgfx::TextureHandle handle = BGFX_INVALID_HANDLE;

	uint32_t size;
	void* data = load(_reader, entry::getAllocator(), _filePath, &size);
	if (NULL != data)
	{
		bimg::ImageContainer* imageContainer = bimg::imageParse(entry::getAllocator(), data, size);

		if (NULL != imageContainer)
		{
			if (NULL != _orientation)
			{
				*_orientation = imageContainer->m_orientation;
			}

			const bgfx::Memory* mem = bgfx::makeRef(
				imageContainer->m_data
				, imageContainer->m_size
				, imageReleaseCb
				, imageContainer
			);
			unload(data);

			if (imageContainer->m_cubeMap)
			{
				handle = bgfx::createTextureCube(
					uint16_t(imageContainer->m_width)
					, 1 < imageContainer->m_numMips
					, imageContainer->m_numLayers
					, bgfx::TextureFormat::Enum(imageContainer->m_format)
					, _flags
					, mem
				);
			}
			else if (1 < imageContainer->m_depth)
			{
				handle = bgfx::createTexture3D(
					uint16_t(imageContainer->m_width)
					, uint16_t(imageContainer->m_height)
					, uint16_t(imageContainer->m_depth)
					, 1 < imageContainer->m_numMips
					, bgfx::TextureFormat::Enum(imageContainer->m_format)
					, _flags
					, mem
				);
			}
			else if (bgfx::isTextureValid(0, false, imageContainer->m_numLayers, bgfx::TextureFormat::Enum(imageContainer->m_format), _flags))
			{
				handle = bgfx::createTexture2D(
					uint16_t(imageContainer->m_width)
					, uint16_t(imageContainer->m_height)
					, 1 < imageContainer->m_numMips
					, imageContainer->m_numLayers
					, bgfx::TextureFormat::Enum(imageContainer->m_format)
					, _flags
					, mem
				);
			}

			if (bgfx::isValid(handle))
			{
				bgfx::setName(handle, _filePath);
			}

			if (NULL != _info)
			{
				bgfx::calcTextureSize(
					*_info
					, uint16_t(imageContainer->m_width)
					, uint16_t(imageContainer->m_height)
					, uint16_t(imageContainer->m_depth)
					, imageContainer->m_cubeMap
					, 1 < imageContainer->m_numMips
					, imageContainer->m_numLayers
					, bgfx::TextureFormat::Enum(imageContainer->m_format)
				);
			}
		}
	}

	return handle;
}

inline bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags = s_defaultTextureSamplerFlags, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL)
{
	return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info, _orientation);
}

void Graphics::SetPlatformData(void* platformData)
{
	entry::s_platformData = *(bgfx::PlatformData*)platformData;
}

void Graphics::Initialize()
{
	entry::init();

	bgfx::Init init;

	init.platformData = entry::s_platformData;

	bgfx::init(init);
	// bgfx::setDebug(BGFX_DEBUG_TEXT);
	// bgfx::setDebug(BGFX_DEBUG_STATS);

	ms_blendFunctions[(int)Graphics::BlendFunction::Default]  = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA);
	ms_blendFunctions[(int)Graphics::BlendFunction::Alpha]    = BGFX_STATE_BLEND_FUNC_SEPARATE(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA, BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA);
	ms_blendFunctions[(int)Graphics::BlendFunction::Add]      = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE);
	ms_blendFunctions[(int)Graphics::BlendFunction::Max]      = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_MAX);
	ms_blendFunctions[(int)Graphics::BlendFunction::Min]      = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_ONE) | BGFX_STATE_BLEND_EQUATION(BGFX_STATE_BLEND_EQUATION_MIN);
	ms_blendFunctions[(int)Graphics::BlendFunction::Multiply] = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_DST_COLOR, BGFX_STATE_BLEND_ZERO) ;

	ms_primitiveTypes[(int)Graphics::PrimitiveType::Triangles] = BGFX_STATE_NONE;
	ms_primitiveTypes[(int)Graphics::PrimitiveType::Lines]     = BGFX_STATE_PT_LINES;
	ms_primitiveTypes[(int)Graphics::PrimitiveType::Points]    = BGFX_STATE_PT_POINTS;

	const bgfx::Caps* caps = bgfx::getCaps();
	ms_originBottomLeft = caps->originBottomLeft;
	ms_texelHalf = bgfx::RendererType::Direct3D9 == caps->rendererType ? 0.5f : 0.0f;

	PosColorVertex::Init();
	PosColorNormalVertex::Init();
	PosColorTexCoord0Vertex::Init();
	PosNormalTexCoord0Vertex::Init();
	PosColorNormalTexCoord0Vertex::Init();
	ImGuiVertex::Init();

	s_defaultDepthFormat =
		  bgfx::isTextureValid(0, false, 1, bgfx::TextureFormat::D32F, BGFX_TEXTURE_RT | s_pointTextureSamplerFlags)
		? bgfx::TextureFormat::D32F
		: bgfx::TextureFormat::D24;

	// Default GUI renderpass
	// TODO: Move it to GUI::Initialize
	bgfx::setViewMode(MaxRenderPassCount() - 1, bgfx::ViewMode::Sequential);

	for (auto shaderName : s_embeddedShaderNameList) {
		s_embeddedEffects[shaderName] = Effect::LoadEffectFromEmbedded(shaderName);
	}
	ms_defaultShader = s_embeddedEffects["basic"].GetShader();

	ms_samplerTex        = CreateUniformSampler("s_tex");
	ms_samplerTexCube    = CreateUniformSampler("s_texCube");
	ms_samplerTexCubeIrr = CreateUniformSampler("s_texCubeIrr");

	ms_uniformTime       = CreateUniformVec4("u_timev");
	ms_uniformResolution = CreateUniformVec4("u_resolutionv");
	ms_uniformCamPos     = CreateUniformVec4("u_camPos");
	ms_uniformMaterial   = CreateUniformVec4("u_material", 2);
	ms_uniformLights     = CreateUniformVec4("u_lights", 8);

	ms_instance = new Graphics();
	ms_renderPassHelperInstance = new Graphics();
}

inline void DestroyUniform(Graphics::UniformHandle handle)
{
	bgfx::UniformHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::Terminate()
{
	delete ms_renderPassHelperInstance;
	ms_renderPassHelperInstance = nullptr;
	delete ms_instance;
	ms_instance = nullptr;

	for (auto& uniform : ms_uniformHashMap) {
		DestroyUniform(uniform.second);
	}
	STL::Clear(ms_uniformHashMap);

	for (auto& embeddedEffect : s_embeddedEffects) {
		DestroyShader(embeddedEffect.second.GetShader());
	}
	STL::Clear(s_embeddedEffects);

	bgfx::shutdown();

	entry::termiate();
}

void Graphics::RenderFrame()
{
	CHERRYSODA_PROFILE_FUNCTION();

	/*
	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(1, 1, 0x0f, "API: %s", bgfx::getRendererName(bgfx::getRendererType()));
	bgfx::dbgTextPrintf(1, 3, 0x0f, "FPS: %d", Engine::Instance()->FPS());
	bgfx::dbgTextPrintf(1, 4, 0x0f, "DeltaTime: %.4f", Engine::Instance()->DeltaTime());
	*/

	bgfx::frame();

	STL::Clear(s_transientVertexBufferStack);
	STL::Clear(s_transientIndexBufferStack);
}

void Graphics::SetLinearTextureSampling()
{
	s_defaultTextureSamplerFlags = s_linearTextureSamplerFlags;
}

void Graphics::SetPointTextureSampling()
{
	s_defaultTextureSamplerFlags = s_pointTextureSamplerFlags;
}

Math::IVec2 Graphics::GetRenderTargetSize(RenderTarget2D* renderTarget)
{
	if (renderTarget) {
		return Math::IVec2(renderTarget->Width(), renderTarget->Height()); 
	}
	else return Engine::Instance()->GetViewSize();
}

void Graphics::UpdateView()
{
#if BGFX_CONFIG_RENDERER_METAL // Temporary fix of the stuck in bgfx::shutdown() on MacOS
	constexpr uint32_t vsyncFlag = BGFX_RESET_VSYNC;
#else
	constexpr uint32_t vsyncFlag = BGFX_RESET_VSYNC | BGFX_RESET_FLIP_AFTER_RENDER;
#endif
	bgfx::reset(Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight(), ms_vsyncEnabled ? vsyncFlag : BGFX_RESET_NONE);
}

void Graphics::SetRenderTarget(RenderTarget2D* renderTarget)
{
	bgfx::FrameBufferHandle handle = BGFX_INVALID_HANDLE;
	if (renderTarget != nullptr) {
		handle = { renderTarget->GetFrameBuffer() };
	}
	bgfx::setViewFrameBuffer(RenderPass(), handle);
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

void Graphics::SetClearDiscard()
{
	bgfx::setViewClear(RenderPass(), BGFX_CLEAR_DISCARD_DEPTH | BGFX_CLEAR_DISCARD_STENCIL);
}

void Graphics::SetRenderPassOrder(STL::Vector<type::UInt16> renderPassOrder)
{
	bgfx::setViewOrder(0, static_cast<uint16_t>(STL::Count(renderPassOrder)), STL::Data(renderPassOrder));
}

void Graphics::Touch()
{
	bgfx::touch(RenderPass());
}

void Graphics::SetVsyncEnabled(bool vsyncEnabled)
{
	ms_vsyncEnabled = vsyncEnabled;
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

void Graphics::SetViewProjectionMatrix(const Math::Mat4& viewMatrix, const Math::Mat4& projMatrix)
{
	bgfx::setViewTransform(RenderPass(), &viewMatrix, &projMatrix);
}

void Graphics::SetTransformMatrix(const Math::Mat4& transformMatrix)
{
	bgfx::setTransform(&transformMatrix);
}

void Graphics::SetMesh(const MeshInterface* mesh)
{
	switch (mesh->GetBufferType()) {
	case Graphics::BufferType::Static:
		SetVertexBuffer(mesh->GetVertexBuffer());
		SetIndexBuffer(mesh->GetIndexBuffer());
		break;
	case Graphics::BufferType::Dynamic:
		SetDynamicVertexBuffer(mesh->GetVertexBuffer(), mesh->VertexBufferSize());
		SetDynamicIndexBuffer(mesh->GetIndexBuffer(), mesh->IndexBufferSize());
		break;
	case Graphics::BufferType::Transient:
		{
			auto vb = mesh->CreateTransientVertexBuffer();
			auto ib = mesh->CreateTransientIndexBuffer();
			SetTransientVertexBuffer(vb);
			SetTransientIndexBuffer(ib, 0, mesh->IndexBufferSize());
		}
		break;
	}
}

void Graphics::SubmitMesh(const MeshInterface* mesh)
{
	if (mesh->VertexBufferSize() == 0) {
		Discard();
		return;
	}
	SetMesh(mesh);
	SetStateDefault(Graphics::BlendFunction::Default, mesh->GetPrimitiveType());
	SubmitOnCurrentRenderPass();
}

void Graphics::SetVertexBuffer(Graphics::VertexBufferHandle vertexBuffer)
{
	bgfx::setVertexBuffer(0, bgfx::VertexBufferHandle{vertexBuffer});
}

void Graphics::SetIndexBuffer(Graphics::IndexBufferHandle indexBuffer)
{
	bgfx::setIndexBuffer(bgfx::IndexBufferHandle{indexBuffer});
}

void Graphics::SetDynamicVertexBuffer(Graphics::DynamicVertexBufferHandle vertexBuffer, size_t vertexAmount)
{
	bgfx::setVertexBuffer(0, bgfx::DynamicVertexBufferHandle{vertexBuffer}, 0, static_cast<type::UInt32>(vertexAmount));
}

void Graphics::SetDynamicIndexBuffer(Graphics::DynamicIndexBufferHandle indexBuffer, size_t indexAmount)
{
	bgfx::setIndexBuffer(bgfx::DynamicIndexBufferHandle{indexBuffer}, 0, static_cast<type::UInt32>(indexAmount));
}

void Graphics::SetTransientVertexBuffer(Graphics::TransientVertexBufferHandle vertexBuffer)
{
	auto i = static_cast<size_t>(vertexBuffer);
	CHERRYSODA_ASSERT(i < STL::Count(s_transientVertexBufferStack), "Transient vertex buffer handle is invalid!\n");
	bgfx::setVertexBuffer(0, STL::Data(s_transientVertexBufferStack) + i);
}

void Graphics::SetTransientIndexBuffer(Graphics::TransientIndexBufferHandle indexBuffer, size_t startIndex, size_t indexAmount)
{
	auto i = static_cast<size_t>(indexBuffer);
	CHERRYSODA_ASSERT(i < STL::Count(s_transientIndexBufferStack), "Transient index buffer handle is invalid!\n");
	bgfx::setIndexBuffer(STL::Data(s_transientIndexBufferStack) + i, startIndex, indexAmount);
}

void Graphics::SetStateDefault(Graphics::BlendFunction blendFunc/* = Graphics::BlendFunction::Normal*/, Graphics::PrimitiveType primType/* = Graphics::PimitiveType::Triangles*/)
{
	constexpr uint64_t stateDefault = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LEQUAL | BGFX_STATE_CULL_CW | BGFX_STATE_MSAA;
	bgfx::setState(stateDefault | ms_blendFunctions[(int)blendFunc] | ms_primitiveTypes[(int)primType]);
}

void Graphics::SetStateNoDepth(Graphics::BlendFunction blendFunc/* = Graphics::BlendFunction::Normal*/, Graphics::PrimitiveType primType/* = Graphics::PimitiveType::Triangles*/)
{
	constexpr uint64_t stateNoDepth = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;
	bgfx::setState(stateNoDepth | ms_blendFunctions[(int)blendFunc] | ms_primitiveTypes[(int)primType]);
}

void Graphics::Submit()
{
	bgfx::submit(RenderPass(), { CurrentShader() });
}

void Graphics::Submit(const Effect* effect)
{
	bgfx::submit(RenderPass(), { effect->m_program });
}

void Graphics::Submit(type::UInt16 renderPass)
{
	bgfx::submit(renderPass, { CurrentShader() });
}

void Graphics::Submit(type::UInt16 renderPass, const Effect* effect)
{
	bgfx::submit(renderPass, { effect->m_program });
}

void Graphics::SubmitOnCurrentRenderPass()
{
	bgfx::submit(CurrentRenderPass(), { CurrentShader() });
}

void Graphics::SubmitOnCurrentRenderPass(const Effect* effect)
{
	bgfx::submit(CurrentRenderPass(), { effect->m_program });
}

void Graphics::Discard()
{
	bgfx::discard();
}

void Graphics::ScreenSpaceQuad(float _width/* = 1.0f*/, float _height/* = 1.0f*/, bool _originBottomLeft/* = false*/)
{
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Definition::s_layout)) {
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Definition::s_layout);
		Graphics::PosColorTexCoord0Vertex* vertex = (Graphics::PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx =  _width;
		const float miny = 0.0f;
		const float maxy = _height * 2.0f;

		const float minu = -1.0f;
		const float maxu =  1.0f;

		const float minv = _originBottomLeft ? 0.0f :  1.0f;
		const float maxv = _originBottomLeft ? 2.0f : -1.0f;

		vertex[0].m_x = minx;
		vertex[0].m_y = miny;
		vertex[0].m_z = zz;
		vertex[0].m_abgr = 0xffffffff;
		vertex[0].m_u = minu;
		vertex[0].m_v = minv;

		vertex[1].m_x = maxx;
		vertex[1].m_y = miny;
		vertex[1].m_z = zz;
		vertex[1].m_abgr = 0xffffffff;
		vertex[1].m_u = maxu;
		vertex[1].m_v = minv;

		vertex[2].m_x = maxx;
		vertex[2].m_y = maxy;
		vertex[2].m_z = zz;
		vertex[2].m_abgr = 0xffffffff;
		vertex[2].m_u = maxu;
		vertex[2].m_v = maxv;

		bgfx::setVertexBuffer(0, &vb);
	}
}

Graphics::IndexBufferHandle Graphics::CreateIndexBuffer(STL::Vector<type::UInt16>& indices)
{
	return bgfx::createIndexBuffer(
		bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices)))
	).idx;
}

Graphics::DynamicIndexBufferHandle Graphics::CreateDynamicIndexBuffer(STL::Vector<type::UInt16>& indices)
{
	return bgfx::createDynamicIndexBuffer(
		bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices))),
		BGFX_BUFFER_ALLOW_RESIZE
	).idx;
}

Graphics::TransientIndexBufferHandle Graphics::CreateTransientIndexBuffer(const STL::Vector<type::UInt16>& indices)
{
	auto indexAmount = static_cast<type::UInt32>(STL::Count(indices));
	if (!indexAmount) return Graphics::InvalidHandle;
	if (indexAmount == bgfx::getAvailTransientIndexBuffer(indexAmount)) {
		bgfx::TransientIndexBuffer ib;
		bgfx::allocTransientIndexBuffer(&ib, indexAmount);
		auto index = (type::UInt16*)ib.data;
		bx::memCopy((void*)index, (void*)STL::Data(indices), STL::ByteSize(indices));
		STL::Add(s_transientIndexBufferStack, ib);
		return Graphics::TransientIndexBufferHandle(STL::Count(s_transientIndexBufferStack) - 1);
	}
	return Graphics::InvalidHandle;
}

Graphics::TransientIndexBufferHandle Graphics::CreateTransientIndexBuffer(const type::UInt16* indices, type::UInt32 indexAmount)
{
	if (!indexAmount) return Graphics::InvalidHandle;
	if (indexAmount == bgfx::getAvailTransientIndexBuffer(indexAmount)) {
		bgfx::TransientIndexBuffer ib;
		bgfx::allocTransientIndexBuffer(&ib, indexAmount);
		auto index = (type::UInt16*)ib.data;
		bx::memCopy((void*)index, (void*)indices, (size_t)indexAmount << 1);
		STL::Add(s_transientIndexBufferStack, ib);
		return Graphics::TransientIndexBufferHandle(STL::Count(s_transientIndexBufferStack) - 1);
	}
	return Graphics::InvalidHandle;
}

void Graphics::UpdateDynamicIndexBuffer(Graphics::DynamicIndexBufferHandle handle, int index, const STL::Vector<type::UInt16>& indices)
{
	bgfx::DynamicIndexBufferHandle hdl = { handle };
	bgfx::update(
		hdl, index, bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices)))
	);
}

Graphics::ShaderHandle Graphics::CreateShaderProgramFromEmbedded(const String& vs, const String& fs)
{
	return loadEmbeddedProgram(vs, fs).idx;
}

Graphics::ShaderHandle Graphics::CreateShaderProgramFromFile(const String& vs, const String& fs)
{
	return loadProgram(vs, fs).idx;
}

const Effect Graphics::GetEmbeddedEffect(const StringID name)
{
	CHERRYSODA_ASSERT_FORMAT(STL::ContainsKey(s_embeddedEffects, name), "Didn't find \"%s\" in embedded shaders\n", name.GetStr().c_str());
	return s_embeddedEffects[name];
}

Graphics::TextureHandle Graphics::CreateTexture(const String& texture, Graphics::TextureInfo* info/* = nullptr */)
{
	bgfx::TextureInfo bInfo;
	Graphics::TextureHandle tex = loadTexture(texture.c_str(), GetTextureSamplerFlags(), 0, &bInfo).idx;
	if (info != nullptr) {
		*info = { bInfo.width, bInfo.height, bInfo.cubeMap };
	}
	CHERRYSODA_ASSERT_FORMAT(tex != Graphics::InvalidHandle, "Texture: \"%s\" loading failed!\n", texture.c_str());
	return tex;
}

Graphics::TextureHandle Graphics::CreateTexture2DFromRGBA(void* data, int width, int height)
{
	const bgfx::Memory* mem = data != nullptr ? bgfx::copy(data, width * height * 4) : nullptr;
	return bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::Enum::RGBA8, GetTextureSamplerFlags(), mem).idx;
}

Graphics::TextureHandle Graphics::CreateTexture2DForColorBuffer(int width, int height)
{
	uint64_t flags = GetTextureSamplerFlags() | BGFX_TEXTURE_RT;
	return bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::Enum::BGRA8, flags).idx;
}

Graphics::TextureHandle Graphics::CreateTexture2DForDepthBuffer(int width, int height)
{
	uint64_t flags = GetTextureSamplerFlags() | BGFX_TEXTURE_RT;
	return bgfx::createTexture2D(width, height, false, 1, s_defaultDepthFormat, flags).idx;
}

Graphics::FrameBufferHandle Graphics::CreateFrameBuffer(int num, const Graphics::TextureHandle* handles)
{
	return bgfx::createFrameBuffer(num, (const bgfx::TextureHandle*)handles).idx;
}

Graphics::UniformHandle Graphics::CreateUniformVec4(const String& uniform, type::UInt16 num)
{
	CHERRYSODA_ASSERT_FORMAT(!STL::ContainsKey(ms_uniformHashMap, uniform), "Uniform '%s' already exists.\n", uniform.c_str());
	Graphics::UniformHandle handle = bgfx::createUniform(uniform.c_str(), bgfx::UniformType::Vec4, num).idx;
	ms_uniformHashMap[uniform] = handle;
	return handle;
}

Graphics::UniformHandle Graphics::CreateUniformMat4(const String& uniform)
{
	CHERRYSODA_ASSERT_FORMAT(!STL::ContainsKey(ms_uniformHashMap, uniform), "Uniform '%s' already exists.\n", uniform.c_str());
	Graphics::UniformHandle handle = bgfx::createUniform(uniform.c_str(), bgfx::UniformType::Mat4).idx;
	ms_uniformHashMap[uniform] = handle;
	return handle;
}

Graphics::UniformHandle Graphics::CreateUniformSampler(const String& sampler)
{
	CHERRYSODA_ASSERT_FORMAT(!STL::ContainsKey(ms_uniformHashMap, sampler), "Uniform '%s' already exists.\n", sampler.c_str());
	Graphics::UniformHandle handle = bgfx::createUniform(sampler.c_str(), bgfx::UniformType::Sampler).idx;
	ms_uniformHashMap[sampler] = handle;
	return handle;
}

void Graphics::DestroyVertexBuffer(VertexBufferHandle handle)
{
	bgfx::VertexBufferHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyIndexBuffer(IndexBufferHandle handle)
{
	bgfx::IndexBufferHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyDynamicVertexBuffer(DynamicVertexBufferHandle handle)
{
	bgfx::DynamicVertexBufferHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyDynamicIndexBuffer(DynamicIndexBufferHandle handle)
{
	bgfx::DynamicIndexBufferHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyShader(ShaderHandle handle)
{
	bgfx::ProgramHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyTexture(TextureHandle handle)
{
	bgfx::TextureHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::SetScissor(int x, int y, int w, int h)
{
	bgfx::setScissor(x, y, w, h);
}

void Graphics::SetEffect(const Effect* effect)
{
	SetShader(effect != nullptr ? effect->GetShader() : Graphics::InvalidHandle);
} 

void Graphics::SetTexture(Graphics::UniformHandle uniform, Graphics::TextureHandle texture)
{
	bgfx::setTexture(0, { uniform }, { texture });
}

void Graphics::SetTexture(type::UInt8 stage, Graphics::UniformHandle uniform, Graphics::TextureHandle texture)
{
	bgfx::setTexture(stage, { uniform }, { texture });
}

void Graphics::SetTexture(const Texture* texture)
{
	SetTexture(ms_samplerTex, texture != nullptr ? texture->GetHandle() : Graphics::InvalidHandle);
}

void Graphics::SetUniform(Graphics::UniformHandle uniform, const void* value, type::UInt16 size/* = 1U*/)
{
	bgfx::setUniform({ uniform }, value, size);
}

void Graphics::SetUniform(StringID uniformName, const void* value, type::UInt16 size/* = 1U*/)
{
	CHERRYSODA_ASSERT_FORMAT(STL::ContainsKey(ms_uniformHashMap, uniformName), "Uniform '%s' doesn't exist.\n", uniformName.GetStr().c_str());
	bgfx::setUniform({ ms_uniformHashMap[uniformName] }, value, size);
}

void Graphics::SetupEngineUniforms()
{
	auto inst = Engine::Instance();
	auto renderer = Draw::GetRenderer();
	Math::Vec2 resolution = Math::Vec2(Graphics::GetRenderTargetSize(renderer->GetRenderTarget()));
	Math::Vec2 surfaceSize = Math::Vec2(resolution.x / resolution.y, 1.f);
	Math::Vec4 timeVec4 = Math::Vec4(inst->GameTime(), inst->DeltaTime(), inst->RawGameTime(), inst->RawDeltaTime());
	Math::Vec4 resolutionVec4 = Math::Vec4(resolution, surfaceSize);
	bgfx::setUniform({ ms_uniformTime },       &timeVec4);
	bgfx::setUniform({ ms_uniformResolution }, &resolutionVec4);
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

static Math::Vec4 s_lightVec4[8];

void Graphics::SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor, bool submit/* = true*/)
{
	s_lightVec4[index * 2] = Math::Vec4(lightPos, 1.0f);
	s_lightVec4[index * 2 + 1] = Math::Vec4(lightColor, 1.0f);
	if (submit) {
		bgfx::setUniform({ ms_uniformLights }, s_lightVec4, 8U);
	}
}

void Graphics::SubmitUniformLight()
{
	bgfx::setUniform({ ms_uniformLights }, s_lightVec4, 8U);
}

void Graphics::SetTextureCube(const TextureCube* texture)
{
	SetTexture(ms_samplerTexCube, texture->GetHandle());
}

void Graphics::SetTextureCubeIrr(const TextureCube* texture)
{
	SetTexture(1, ms_samplerTexCubeIrr, texture->GetHandle());	
}

type::UInt64 Graphics::ms_blendFunctions[(int)BlendFunction::Count];
type::UInt64 Graphics::ms_primitiveTypes[(int)PrimitiveType::Count];
type::UInt16 Graphics::ms_maxRenderPassCount = BGFX_CONFIG_MAX_VIEWS;
bool Graphics::ms_vsyncEnabled = true;

bool Graphics::ms_originBottomLeft = false;
float Graphics::ms_texelHalf = 0.f;

Graphics::ShaderHandle Graphics::ms_defaultShader         = Graphics::InvalidHandle;
Graphics::ShaderHandle Graphics::ms_defaultShaderOverride = Graphics::InvalidHandle;

STL::HashMap<StringID, Graphics::UniformHandle> Graphics::ms_uniformHashMap;

Graphics::UniformHandle Graphics::ms_samplerTex        = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_samplerTexCube    = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_samplerTexCubeIrr = Graphics::InvalidHandle;

Graphics::UniformHandle Graphics::ms_uniformTime       = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformResolution = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformCamPos     = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformLights     = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformMaterial   = Graphics::InvalidHandle;

Graphics* Graphics::ms_instance = nullptr;
Graphics* Graphics::ms_renderPassHelperInstance = nullptr;

#define CHERRYSODA_CREATE_VERTEX_BUFFER(VERTEX_D) \
Graphics::VertexBufferHandle Graphics::CreateVertexBuffer(const STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	return bgfx::createVertexBuffer( \
		bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))), \
		VERTEX_D::s_layout \
	).idx; \
}

#define CHERRYSODA_CREATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D) \
Graphics::DynamicVertexBufferHandle Graphics::CreateDynamicVertexBuffer(const STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	return bgfx::createDynamicVertexBuffer( \
		bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))), \
		VERTEX_D::s_layout, BGFX_BUFFER_ALLOW_RESIZE \
	).idx; \
}

#define CHERRYSODA_UPDATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D) \
void Graphics::UpdateDynamicVertexBuffer(Graphics::DynamicVertexBufferHandle handle, int index, const STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	bgfx::DynamicVertexBufferHandle hdl = { handle }; \
	bgfx::update( \
		hdl, index, bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))) \
	); \
}

#define CHERRYSODA_CREATE_TRANSIENT_VERTEX_BUFFER(VERTEX_D) \
Graphics::TransientVertexBufferHandle Graphics::CreateTransientVertexBuffer(const STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	auto vertexAmount = static_cast<type::UInt32>(STL::Count(vertices)); \
	if (!vertexAmount) return Graphics::InvalidHandle; \
	if (vertexAmount == bgfx::getAvailTransientVertexBuffer(vertexAmount, VERTEX_D::s_layout)) { \
		bgfx::TransientVertexBuffer vb; \
		bgfx::allocTransientVertexBuffer(&vb, vertexAmount, VERTEX_D::s_layout); \
		auto vertex = (VERTEX_D::VertexType*)vb.data; \
		bx::memCopy((void*)vertex, (void*)STL::Data(vertices), STL::ByteSize(vertices)); \
		STL::Add(s_transientVertexBufferStack, vb); \
		return (Graphics::TransientVertexBufferHandle)(STL::Count(s_transientVertexBufferStack) - 1); \
	} \
	return Graphics::InvalidHandle; \
} \
Graphics::TransientVertexBufferHandle Graphics::CreateTransientVertexBuffer(const VERTEX_D::VertexType* vertices, type::UInt32 vertexAmount) \
{ \
	if (!vertexAmount) return Graphics::InvalidHandle; \
	if (vertexAmount == bgfx::getAvailTransientVertexBuffer(vertexAmount, VERTEX_D::s_layout)) { \
		bgfx::TransientVertexBuffer vb; \
		bgfx::allocTransientVertexBuffer(&vb, vertexAmount, VERTEX_D::s_layout); \
		auto vertex = (VERTEX_D::VertexType*)vb.data; \
		bx::memCopy((void*)vertex, (void*)vertices, sizeof(VERTEX_D::VertexType) * vertexAmount); \
		STL::Add(s_transientVertexBufferStack, vb); \
		return (Graphics::TransientVertexBufferHandle)(STL::Count(s_transientVertexBufferStack) - 1); \
	} \
	return Graphics::InvalidHandle; \
}

#define CHERRYSODA_VERTEX_IMPLEMENTATION(VERTEX_D) \
CHERRYSODA_CREATE_VERTEX_BUFFER(VERTEX_D); \
CHERRYSODA_CREATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D); \
CHERRYSODA_UPDATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D); \
CHERRYSODA_CREATE_TRANSIENT_VERTEX_BUFFER(VERTEX_D);

CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorDefinition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorNormalDefinition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorTexCoord0Definition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosNormalTexCoord0Definition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorNormalTexCoord0Definition);
CHERRYSODA_VERTEX_IMPLEMENTATION(ImGuiVertexDefinition);
