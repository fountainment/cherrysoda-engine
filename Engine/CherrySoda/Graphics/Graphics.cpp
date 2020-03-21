#include <CherrySoda/Graphics/Graphics.h>

#include <CherrySoda/Graphics/Effect.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Camera.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>
#include <CherrySoda/Util/String.h>

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/file.h>
#include <bx/allocator.h>
#include <bimg/bimg.h>
#include <bimg/decode.h>

#include <sstream>
#include <fstream>

using cherrysoda::Graphics;

using cherrysoda::Camera;
using cherrysoda::Color;
using cherrysoda::Effect;
using cherrysoda::Engine;
using cherrysoda::Math;
using cherrysoda::MeshInterface;
using cherrysoda::Texture;
using cherrysoda::Texture2D;
using cherrysoda::TextureCube;
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringUtil;

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

bgfx::VertexLayout PosColorDefinition::s_layout;
bgfx::VertexLayout PosColorNormalDefinition::s_layout;
bgfx::VertexLayout PosColorTexCoord0Definition::s_layout;

namespace entry {

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

bgfx::ProgramHandle loadProgram(const String& vs, const String& fs)
{
	String path_prefix = "assets/shaders/" CHERRYSODA_SHADER_PLATFORM "/";
	bgfx::ShaderHandle vsh = loadShader(path_prefix + vs + ".bin");
	bgfx::ShaderHandle fsh = loadShader(path_prefix + fs + ".bin");
	return bgfx::createProgram(vsh, fsh, true);
}

void* load(bx::FileReaderI* _reader, bx::AllocatorI* _allocator, const char* _filePath, uint32_t* _size)
{
	if (bx::open(_reader, _filePath))
	{
		uint32_t size = (uint32_t)bx::getSize(_reader);
		void* data = BX_ALLOC(_allocator, size);
		bx::read(_reader, data, size);
		bx::close(_reader);
		if (NULL != _size)
		{
			*_size = size;
		}
		return data;
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

bgfx::TextureHandle loadTexture(const char* _name, uint64_t _flags = BGFX_TEXTURE_NONE|BGFX_SAMPLER_NONE, uint8_t _skip = 0, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL)
{
	return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info, _orientation);
}

bgfx::TextureHandle loadTexture(const char* _name, bgfx::TextureInfo* _info = NULL, bimg::Orientation::Enum* _orientation = NULL, uint64_t _flags = BGFX_TEXTURE_NONE|BGFX_SAMPLER_NONE, uint8_t _skip = 0)
{
	return loadTexture(entry::getFileReader(), _name, _flags, _skip, _info, _orientation);
}

void Graphics::Initialize()
{
	entry::init();

	bgfx::init();
	// bgfx::setDebug(BGFX_DEBUG_TEXT);

	Graphics::PosColorVertex::Init();
	Graphics::PosColorNormalVertex::Init();
	Graphics::PosColorTexCoord0Vertex::Init();

	// ms_defaultShader = Graphics::CreateShaderProgram("vs_mypbr", "fs_mypbr");

#ifdef CHERRYSODA_PROFILE
	ms_vsyncEnabled = false;
#endif

	ms_samplerTex        = CreateUniformSampler("s_tex");
	ms_samplerTexCube    = CreateUniformSampler("s_texCube");
	ms_samplerTexCubeIrr = CreateUniformSampler("s_texCubeIrr");

	ms_uniformCamPos   = CreateUniformVec4("u_camPos");
	ms_uniformMaterial = CreateUniformVec4("u_material", 2);
	ms_uniformLights   = CreateUniformVec4("u_lights", 8);
	ms_uniformParams   = CreateUniformVec4("u_params", 16);

	ms_instance = new Graphics();
	ms_renderPassHelperInstance = new Graphics();
}

void Graphics::Terminate()
{
	bgfx::shutdown();
}

void Graphics::RenderFrame()
{
	/*
	bgfx::dbgTextClear();
	bgfx::dbgTextPrintf(1, 1, 0x0f, "API: %s", bgfx::getRendererName(bgfx::getRendererType()));
	bgfx::dbgTextPrintf(1, 3, 0x0f, "FPS: %d", Engine::Instance()->FPS());
	bgfx::dbgTextPrintf(1, 4, 0x0f, "DeltaTime: %.4f", Engine::Instance()->DeltaTime());
	*/

	bgfx::frame();
}

void Graphics::UpdateView()
{
	bgfx::reset(Engine::Instance()->GetWidth(), Engine::Instance()->GetHeight(), ms_vsyncEnabled ? BGFX_RESET_VSYNC : BGFX_RESET_NONE);
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

void Graphics::SetRenderPassOrder(STL::Vector<cherrysoda::type::UInt16> renderPassOrder)
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

void Graphics::SetTransformMatrix(const Math::Mat4& transformMatrix)
{
	bgfx::setTransform(&transformMatrix);
}

void Graphics::SetMesh(MeshInterface* mesh)
{
	switch (mesh->GetBufferType()) {
	case MeshInterface::BufferType::Static:
		SetVertexBuffer(mesh->GetVertexBuffer());
		SetIndexBuffer(mesh->GetIndexBuffer());
		break;
	case MeshInterface::BufferType::Dynamic:
		SetDynamicVertexBuffer(mesh->GetVertexBuffer(), mesh->VertexBufferSize());
		SetDynamicIndexBuffer(mesh->GetIndexBuffer(), mesh->IndexBufferSize());
		break;
	case MeshInterface::BufferType::Transient:
		mesh->SetTransientBuffer();
		break;
	}
}

void Graphics::SetVertexBuffer(Graphics::VertexBufferHandle vertexBuffer)
{
	bgfx::setVertexBuffer(0, bgfx::VertexBufferHandle{vertexBuffer});
}

void Graphics::SetIndexBuffer(Graphics::IndexBufferHandle indexBuffer)
{
	bgfx::setIndexBuffer(bgfx::IndexBufferHandle{indexBuffer});
}

void Graphics::SetDynamicVertexBuffer(Graphics::VertexBufferHandle vertexBuffer, size_t vertexAmount)
{
	bgfx::setVertexBuffer(0, bgfx::DynamicVertexBufferHandle{vertexBuffer}, 0, static_cast<type::UInt32>(vertexAmount));
}

void Graphics::SetDynamicIndexBuffer(Graphics::IndexBufferHandle indexBuffer, size_t indexAmount)
{
	bgfx::setIndexBuffer(bgfx::DynamicIndexBufferHandle{indexBuffer}, 0, static_cast<type::UInt32>(indexAmount));
}

void Graphics::SetStateDefault()
{
	bgfx::setState(BGFX_STATE_DEFAULT);
}

void Graphics::SetStateNoDepth()
{
	bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
}

void Graphics::Submit()
{
	bgfx::submit(RenderPass(), { CurrentShader() });
}

void Graphics::Submit(Effect* effect)
{
	bgfx::submit(RenderPass(), { effect->m_program });
}

void Graphics::Submit(cherrysoda::type::UInt16 renderPass)
{
	bgfx::submit(renderPass, { CurrentShader() });
}

void Graphics::Submit(cherrysoda::type::UInt16 renderPass, Effect* effect)
{
	bgfx::submit(renderPass, { effect->m_program });
}

void Graphics::SubmitOnCurrentRenderPass()
{
	bgfx::submit(CurrentRenderPass(), { CurrentShader() });
}

void Graphics::SubmitOnCurrentRenderPass(Effect* effect)
{
	bgfx::submit(CurrentRenderPass(), { effect->m_program });
}

void Graphics::ScreenSpaceQuad(float _textureWidth, float _textureHeight, bool _originBottomLeft, float _width, float _height)
{
	float s_texelHalf = 0.f;
	if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Definition::s_layout)) {
		bgfx::TransientVertexBuffer vb;
		bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Definition::s_layout);
		Graphics::PosColorTexCoord0Vertex* vertex = (Graphics::PosColorTexCoord0Vertex*)vb.data;

		const float zz = 0.0f;

		const float minx = -_width;
		const float maxx =  _width;
		const float miny = 0.0f;
		const float maxy = _height*2.0f;

		const float texelHalfW = s_texelHalf/_textureWidth;
		const float texelHalfH = s_texelHalf/_textureHeight;
		const float minu = -1.0f + texelHalfW;
		const float maxu =  1.0f + texelHalfW;

		float minv = texelHalfH;
		float maxv = 2.0f + texelHalfH;

		if (_originBottomLeft)
		{
			float temp = minv;
			minv = maxv;
			maxv = temp;

			minv -= 1.0f;
			maxv -= 1.0f;
		}

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


Graphics::IndexBufferHandle Graphics::CreateIndexBuffer(STL::Vector<cherrysoda::type::UInt16>& indices)
{
	return bgfx::createIndexBuffer(
		bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices)))
	).idx;
}

Graphics::DynamicIndexBufferHandle Graphics::CreateDynamicIndexBuffer(STL::Vector<cherrysoda::type::UInt16>& indices)
{
	return bgfx::createDynamicIndexBuffer(
		bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices))),
		BGFX_BUFFER_ALLOW_RESIZE
	).idx;
}

void Graphics::UpdateDynamicIndexBuffer(Graphics::DynamicIndexBufferHandle handle, int index, const STL::Vector<cherrysoda::type::UInt16>& indices)
{
	bgfx::DynamicIndexBufferHandle hdl = { handle };
	bgfx::update(
		hdl, index, bgfx::makeRef(STL::Data(indices), static_cast<uint32_t>(STL::ByteSize(indices)))
	);
}

Graphics::ShaderHandle Graphics::CreateShaderProgram(const String& vs, const String& fs)
{
	return loadProgram(vs, fs).idx;
}

Graphics::TextureHandle Graphics::CreateTexture(const String& texture, Graphics::TextureInfo* info/* = nullptr */)
{
	bgfx::TextureInfo bInfo;
	Graphics::TextureHandle tex = loadTexture(texture.c_str(), &bInfo).idx;
	if (info != nullptr) {
		*info = { bInfo.width, bInfo.height, bInfo.cubeMap };
	}
	return tex;
}

Graphics::UniformHandle Graphics::CreateUniformVec4(const String& uniform, cherrysoda::type::UInt16 num)
{
	return bgfx::createUniform(uniform.c_str(), bgfx::UniformType::Vec4, num).idx;
}

Graphics::UniformHandle Graphics::CreateUniformMat4(const String& uniform)
{
	return bgfx::createUniform(uniform.c_str(), bgfx::UniformType::Mat4).idx;
}

Graphics::UniformHandle Graphics::CreateUniformSampler(const String& sampler)
{
	return bgfx::createUniform(sampler.c_str(), bgfx::UniformType::Sampler).idx;
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
	bgfx::ShaderHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyTexture(TextureHandle handle)
{
	bgfx::TextureHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::DestroyUniform(UniformHandle handle)
{
	bgfx::UniformHandle hdl = { handle };
	bgfx::destroy(hdl);
}

void Graphics::SetEffect(Effect* effect)
{
	SetShader(effect != nullptr ? effect->GetShader() : Graphics::InvalidHandle);
} 

void Graphics::SetTexture(Graphics::UniformHandle uniform, Graphics::TextureHandle texture)
{
	bgfx::setTexture(0, { uniform }, { texture });
}

void Graphics::SetTexture(cherrysoda::type::UInt8 stage, Graphics::UniformHandle uniform, Graphics::TextureHandle texture)
{
	bgfx::setTexture(stage, { uniform }, { texture });
}

void Graphics::SetTexture(Texture* texture)
{
	SetTexture(ms_samplerTex, texture->m_texture);
}

void Graphics::SetUniform(Graphics::UniformHandle uniform, const void* value, cherrysoda::type::UInt16 size/* = 1U*/)
{
	bgfx::setUniform({ uniform }, value, size);
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

void Graphics::SetTextureCube(TextureCube* texture)
{
	SetTexture(ms_samplerTexCube, texture->m_texture);	
}

void Graphics::SetTextureCubeIrr(TextureCube* texture)
{
	SetTexture(1, ms_samplerTexCubeIrr, texture->m_texture);	
}

bool Graphics::ms_vsyncEnabled = true;

Graphics::ShaderHandle Graphics::ms_defaultShader         = Graphics::InvalidHandle;
Graphics::ShaderHandle Graphics::ms_defaultShaderOverride = Graphics::InvalidHandle;

Graphics::UniformHandle Graphics::ms_samplerTex        = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_samplerTexCube    = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_samplerTexCubeIrr = Graphics::InvalidHandle;

Graphics::UniformHandle Graphics::ms_uniformCamPos   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformLights   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformMaterial = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformParams   = Graphics::InvalidHandle;

Graphics* Graphics::ms_instance = nullptr;
Graphics* Graphics::ms_renderPassHelperInstance = nullptr;

#define CHERRYSODA_CREATE_VERTEX_BUFFER(VERTEX_D) \
Graphics::VertexBufferHandle Graphics::CreateVertexBuffer(STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	return bgfx::createVertexBuffer( \
		bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))), \
		VERTEX_D::s_layout \
	).idx; \
}

#define CHERRYSODA_CREATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D) \
Graphics::DynamicVertexBufferHandle Graphics::CreateDynamicVertexBuffer(STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	return bgfx::createDynamicVertexBuffer( \
		bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))), \
		VERTEX_D::s_layout, BGFX_BUFFER_ALLOW_RESIZE \
	).idx; \
}

#define CHERRYSODA_UPDATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D) \
void Graphics::UpdateDynamicVertexBuffer(Graphics::DynamicVertexBufferHandle handle, int index, STL::Vector<VERTEX_D::VertexType>& vertices) \
{ \
	bgfx::DynamicVertexBufferHandle hdl = { handle }; \
	bgfx::update( \
		hdl, index, bgfx::makeRef(STL::Data(vertices), static_cast<uint32_t>(STL::ByteSize(vertices))) \
	); \
}

#define CHERRYSODA_VERTEX_IMPLEMENTATION(VERTEX_D) \
CHERRYSODA_CREATE_VERTEX_BUFFER(VERTEX_D); \
CHERRYSODA_CREATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D); \
CHERRYSODA_UPDATE_DYNAMIC_VERTEX_BUFFER(VERTEX_D);

CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorDefinition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorNormalDefinition);
CHERRYSODA_VERTEX_IMPLEMENTATION(PosColorTexCoord0Definition);
