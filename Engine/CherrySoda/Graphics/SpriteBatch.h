#ifndef _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_
#define _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Graphics/Mesh.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/STL.h>

namespace cherrysoda {

enum class SpriteEffects : type::UInt8
{
	None = 0,
	FlipHorizontally = 1,
	FlipVertically = 2
};

DECLARE_ENUM_FLAG(SpriteEffects);

class SpriteBatch : Mesh<Graphics::PosColorTexCoord0Vertex>
{
public:
	SpriteBatch()
	{ 
		SetBufferType(BufferType::Transient);
	}

	void Begin() { Clear(); }

	void Draw(const Texture2D& tex, const Math::Vec2& pos, const Math::IRectangle& rect, \
			const Color& color, float rotation, const Math::Vec2& origin, const Math::Vec2& scale, \
			SpriteEffects effects, float layerDepth)
	{
		if (VertexAmount() && tex.GetHandle() != m_previousTexture.GetHandle())	{
			End(); Begin();
		}
		float leftUV   = (float)rect.Left()   / (float)tex.Width();
		float rightUV  = (float)rect.Right()  / (float)tex.Width();
		float bottomUV = (float)rect.Bottom() / (float)tex.Height();
		float topUV    = (float)rect.Top()    / (float)tex.Height();
		if ((effects & SpriteEffects::FlipVertically) == SpriteEffects::FlipVertically) {
			STL::Swap(bottomUV, topUV);
		}
		if ((effects & SpriteEffects::FlipHorizontally) == SpriteEffects::FlipVertically) {
			STL::Swap(leftUV, rightUV);
		}
		if (scale == Vec2_One && rotation == 0.f) {
			AddQuad(
				MK_VERT(Math::Vec3((pos - origin + Math::Vec2(rect.Width(), 0.f) ) * scale, 0.f), color, Math::Vec2(rightUV, bottomUV)),
				MK_VERT(Math::Vec3((pos - origin + Math::Vec2(rect.Size())       ) * scale, 0.f), color, Math::Vec2(rightUV, topUV)),
				MK_VERT(Math::Vec3((pos - origin                                 ) * scale, 0.f), color, Math::Vec2(leftUV,  bottomUV)),
				MK_VERT(Math::Vec3((pos - origin + Math::Vec2(0.f, rect.Height())) * scale, 0.f), color, Math::Vec2(leftUV,  topUV))
			);
		}
		else {
			// TODO: TransformMatrix
		}
		m_previousTexture = tex;
	}

	void End()
	{
		if (VertexAmount()) {
			SubmitBuffer();
			Graphics::SetTexture(&m_previousTexture);
			Graphics::SetTransformMatrix(Math_Identity<Math::Mat4>());
			Graphics::SetMesh(this);
			Graphics::SetStateDefault();
			Graphics::SubmitOnCurrentRenderPass();
		}
		else {
			Graphics::Discard();
		}
	}

private:
	Texture2D m_previousTexture;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_SPRITEBATCH_H_
