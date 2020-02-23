#ifndef _CHERRYSODA_GRAPHICS_TEXTURE_H_
#define _CHERRYSODA_GRAPHICS_TEXTURE_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Texture
{
public:
	static Texture FromFile(const String& filename);

protected:
	friend class Graphics;

	Graphics::TextureHandle m_texture = Graphics::InvalidHandle;
};

class Texture2D : public Texture
{
public:
	static Texture2D FromFile(const String& filename);

private:
	int m_width;
	int m_height;
};

class TextureCube : public Texture
{
public:
	static TextureCube FromFile(const String& filename);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_TEXTURE_H_
