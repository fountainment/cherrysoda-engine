#include <CherrySoda/Graphics/Texture.h>

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::Graphics;
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

TextureCube TextureCube::FromFile(const String& filename)
{
	TextureCube result;
	Graphics::TextureInfo info;
	result.m_texture = Graphics::CreateTexture(filename, &info);
	CHERRYSODA_ASSERT(info.m_cubeMap, CHERRYSODA_FORMAT("Loading texture \"%s\" is not a cubemap!\n", filename.c_str()));
	return result;
}