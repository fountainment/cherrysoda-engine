#include <CherrySoda/Graphics/Texture.h>

#include <CherrySoda/Util/String.h>

using cherrysoda::Texture;
using cherrysoda::String;

Texture Texture::FromFile(const String& filename)
{
	Texture result;
	result.m_texture = Graphics::CreateTexture(filename);	
	return result;
}