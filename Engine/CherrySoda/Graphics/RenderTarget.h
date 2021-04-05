#ifndef _CHERRYSODA_GRAPHICS_RENDERTARGET_H_
#define _CHERRYSODA_GRAPHICS_RENDERTARGET_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>

namespace cherrysoda {

class RenderTarget2D
{
public:
	RenderTarget2D(int width, int height);

	inline int Width() const { return m_texture.Width(); }
	inline int Height() const { return m_texture.Height(); }
	inline Texture2D GetTexture2D() const { return m_texture; }

private:
	friend class Graphics;

	inline Graphics::FrameBufferHandle GetFrameBuffer() const { return m_frameBuffer; }

	Texture2D m_texture;
	Texture2D m_depthTexture;
	Graphics::FrameBufferHandle m_frameBuffer;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_RENDERTARGET_H_
