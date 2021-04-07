#include <CherrySoda/Graphics/Texture.h>

#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::Texture;
using cherrysoda::Texture2D;
using cherrysoda::TextureCube;
using cherrysoda::String;

Texture Texture::FromFile(const String& filename)
{
	Texture result;
	result.m_texture = Graphics::CreateTexture(filename);
	return result;
}

Texture2D Texture2D::FromFile(const String& filename)
{
	Texture2D result;
	Graphics::TextureInfo info;
	result.m_texture = Graphics::CreateTexture(filename, &info);
	result.m_width = info.m_width;
	result.m_height = info.m_height;
	return result;
}

Texture2D Texture2D::FromRGBA(void* data, int width, int height)
{
	Texture2D result;
	result.m_texture = Graphics::CreateTexture2DFromRGBA(data, width, height);
	result.m_width = width;
	result.m_height = height;
	return result;
}

Texture2D Texture2D::ForColorBuffer(int width, int height)
{
	Texture2D result;
	result.m_texture = Graphics::CreateTexture2DForColorBuffer(width, height);
	result.m_width = width;
	result.m_height = height;
	return result;
}

Texture2D Texture2D::ForDepthBuffer(int width, int height)
{
	Texture2D result;
	result.m_texture = Graphics::CreateTexture2DForDepthBuffer(width, height);
	result.m_width = width;
	result.m_height = height;
	return result;
}

TextureCube TextureCube::FromFile(const String& filename)
{
	TextureCube result;
	Graphics::TextureInfo info;
	result.m_texture = Graphics::CreateTexture(filename, &info);
	CHERRYSODA_ASSERT(info.m_cubeMap, CHERRYSODA_FORMAT("Loading texture \"%s\" is not a cubemap!\n", filename.c_str()));
	return result;
}
