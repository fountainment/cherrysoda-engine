#include <CherrySoda/Graphics/RenderTarget.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>

namespace cherrysoda {

RenderTarget2D::RenderTarget2D(int width, int height)
{
	m_texture = Texture2D::ForColorBuffer(width, height);
	m_depthTexture = Texture2D::ForDepthBuffer(width, height);
	const Graphics::TextureHandle handles[] = { m_texture.GetHandle(), m_depthTexture.GetHandle() };
	m_frameBuffer = Graphics::CreateFrameBuffer(2, handles);
}

RenderTarget2D::~RenderTarget2D()
{
	Dispose();
}

void RenderTarget2D::Dispose()
{
	if (m_frameBuffer != Graphics::InvalidHandle) {
		Graphics::DestroyFrameBuffer(m_frameBuffer);
		m_frameBuffer = Graphics::InvalidHandle;
	}
	m_texture.Dispose();
	m_depthTexture.Dispose();
}

} // namespace cherrysoda
