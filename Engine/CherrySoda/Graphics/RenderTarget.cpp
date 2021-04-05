#include <CherrySoda/Graphics/RenderTarget.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>

using cherrysoda::RenderTarget2D;

using cherrysoda::Graphics;
using cherrysoda::Texture2D;

RenderTarget2D::RenderTarget2D(int width, int height)
{
	m_texture = Texture2D::ForRenderTarget(width, height);
	const Graphics::TextureHandle handles[] = { m_texture.GetHandle() };
	m_frameBuffer = Graphics::CreateFrameBuffer(1, handles);
}
