#ifndef _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_
#define _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/SpriteEffects.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Util/NumType.h>


namespace cherrysoda {

class SpriteBatch : Mesh<Graphics::PosColorTexCoord0Vertex>
{
public:
	SpriteBatch() { SetBufferType(Graphics::BufferType::Transient); }

	inline void Begin() { Clear(); }

	void Draw(const Texture2D& tex, const Math::Vec2& pos, const Math::IRectangle& rect,
			const Color& color, float rotation, const Math::Vec2& origin, const Math::Vec2& scale,
			SpriteEffects effects, float layerDepth);

	void End();

private:
	Texture2D m_previousTexture;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_
