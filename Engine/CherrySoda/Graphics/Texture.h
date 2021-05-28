#ifndef _CHERRYSODA_GRAPHICS_TEXTURE_H_
#define _CHERRYSODA_GRAPHICS_TEXTURE_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

namespace cherrysoda {

class Texture
{
public:
	static Texture FromFile(const String& filename);

	Texture() = default;

	inline Graphics::TextureHandle GetHandle() const { return m_texture; }
	inline bool IsValid() const { return m_texture != Graphics::InvalidHandle; }
	inline void Dispose() { if (IsValid()) { Graphics::DestroyTexture(GetHandle()); m_texture = Graphics::InvalidHandle; } }

protected:
	Graphics::TextureHandle m_texture = Graphics::InvalidHandle;
};

class Texture2D : public Texture
{
public:
	static Texture2D FromFile(const String& filename);
	static Texture2D FromRGBA(void* data, int width, int height);
	static Texture2D ForColorBuffer(int width, int height);
	static Texture2D ForDepthBuffer(int width, int height);

	Texture2D() = default;

	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }
	inline const Math::IVec2 Size() const { return Math::IVec2(Width(), Height()); }
	inline int TotalPixels() const { return Width() * Height(); }

private:
	int m_width  = 0;
	int m_height = 0;
};

class Texture3D : public Texture
{
	// TODO: Add Texture3D
};

class TextureCube : public Texture
{
public:
	static TextureCube FromFile(const String& filename);

	TextureCube() = default;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_TEXTURE_H_
