#ifndef _CHERRYSODA_GRAPHICS_TEXTURE_H_
#define _CHERRYSODA_GRAPHICS_TEXTURE_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Texture
{
public:
	static Texture FromFile(const String& filename);

private:
	friend class Graphics;

	Graphics::TextureHandle m_texture = Graphics::InvalidHandle;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_TEXTURE_H_