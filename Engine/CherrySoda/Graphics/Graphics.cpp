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
using cherrysoda::STL;
using cherrysoda::String;
using cherrysoda::StringUtil;

static bgfx::VertexLayout s_posColorLayout;
static bgfx::VertexLayout s_posColorNormalLayout;

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

		const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
		delete [] data;
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

Graphics::Graphics()
{
}

void Graphics::Init()
{
	entry::init();

	bgfx::init();
	// bgfx::setDebug(BGFX_DEBUG_TEXT);

	Graphics::PosColorVertex::Init();
	Graphics::PosColorNormalVertex::Init();

	ms_defaultShader = Graphics::CreateShaderProgram("vs_mypbr", "fs_mypbr");

	ms_samplerTexCube    = bgfx::createUniform("s_texCube", bgfx::UniformType::Sampler).idx;
	ms_samplerTexCubeIrr = bgfx::createUniform("s_texCubeIrr", bgfx::UniformType::Sampler).idx;

	ms_uniformCamPos   = bgfx::createUniform("u_camPos", bgfx::UniformType::Vec4).idx;
	ms_uniformMaterial = bgfx::createUniform("u_material", bgfx::UniformType::Vec4, 2).idx;
	ms_uniformLights   = bgfx::createUniform("u_lights", bgfx::UniformType::Vec4, 8).idx;
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
	bgfx::setVertexBuffer(0, bgfx::VertexBufferHandle{vertexBuffer});
}

void Graphics::SetIndexBuffer(Graphics::IndexBufferHandle indexBuffer)
{
	bgfx::setIndexBuffer(bgfx::IndexBufferHandle{indexBuffer});
}

void Graphics::Submit()
{
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(RenderPass(), { ms_defaultShaderOverride != Graphics::InvalidHandle ? ms_defaultShaderOverride : ms_defaultShader });
}

void Graphics::Submit(Effect* effect)
{
	bgfx::setState(BGFX_STATE_DEFAULT);
	bgfx::submit(RenderPass(), { effect->m_program });
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

Graphics::ShaderHandle Graphics::CreateShaderProgram(const String& vs, const String& fs)
{
	return loadProgram(vs, fs).idx;
}

Graphics::TextureHandle Graphics::CreateTexture(const String& texture)
{
	return loadTexture(texture.c_str()).idx;
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
	SetTexture(ms_samplerTexCube, texture->m_texture);
}

void Graphics::SetUniform(Graphics::UniformHandle uniform, const void* value, cherrysoda::type::UInt16 size)
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

void Graphics::SetUniformLight(int index, const Math::Vec3& lightPos, const Math::Vec3& lightColor)
{
	static Math::Vec4 lightVec4[8];
	lightVec4[index * 2] = Math::Vec4(lightPos, 1.0f);
	lightVec4[index * 2 + 1] = Math::Vec4(lightColor, 1.0f);
	bgfx::setUniform({ ms_uniformLights }, lightVec4, 8U);
}

void Graphics::SetSamplerTexCube(Texture* texture)
{
	SetTexture(ms_samplerTexCube, texture->m_texture);	
}

void Graphics::SetSamplerTexCubeIrr(Texture* texture)
{
	SetTexture(1, ms_samplerTexCubeIrr, texture->m_texture);	
}

Graphics::ShaderHandle Graphics::ms_defaultShader         = Graphics::InvalidHandle;
Graphics::ShaderHandle Graphics::ms_defaultShaderOverride = Graphics::InvalidHandle;

Graphics::UniformHandle Graphics::ms_samplerTexCube    = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_samplerTexCubeIrr = Graphics::InvalidHandle;

Graphics::UniformHandle Graphics::ms_uniformCamPos   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformLights   = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformMaterial = Graphics::InvalidHandle;
Graphics::UniformHandle Graphics::ms_uniformParams   = Graphics::InvalidHandle;

Graphics* Graphics::ms_instance = nullptr;