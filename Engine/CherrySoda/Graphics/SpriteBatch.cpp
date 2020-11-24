#include <CherrySoda/Graphics/SpriteBatch.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::SpriteBatch;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::SpriteEffects;
using cherrysoda::STL;
using cherrysoda::Texture2D;

void SpriteBatch::Draw(const Texture2D& tex, const Math::Vec2& pos, const Math::IRectangle& rect, \
					const Color& color, float rotation, const Math::Vec2& origin, const Math::Vec2& scale, \
					SpriteEffects effects, float layerDepth)
{
	if (VertexAmount() && tex.GetHandle() != m_previousTexture.GetHandle())	{
		End(); Begin();
	}
	float leftUV   = ((float)rect.Left()   + Graphics::TexelHalf()) / (float)tex.Width();
	float rightUV  = ((float)rect.Right()  + Graphics::TexelHalf()) / (float)tex.Width();
	float bottomUV = ((float)rect.Top()    + Graphics::TexelHalf()) / (float)tex.Height();
	float topUV    = ((float)rect.Bottom() + Graphics::TexelHalf()) / (float)tex.Height();
	if ((effects & SpriteEffects::FlipVertically) == SpriteEffects::FlipVertically) {
		STL::Swap(bottomUV, topUV);
	}
	if ((effects & SpriteEffects::FlipHorizontally) == SpriteEffects::FlipVertically) {
		STL::Swap(leftUV, rightUV);
	}
	const Math::Vec2 rPos = ((effects & SpriteEffects::RoundRenderingPosition) == SpriteEffects::RoundRenderingPosition) ? Math_Round(pos - origin) : pos - origin;
	const Math::Mat4 rotationMat = rotation == 0 ? Math_Identity<Math::Mat4>() : Math_Rotate(Math_Identity<Math::Mat4>(), rotation, Vec3_ZUp);
	AddQuad(
		MK_VERT(Math::Vec3(rotationMat * Math::Vec4((rPos + Math::Vec2(rect.Width(), 0.f) ) * scale, 0.f, 1.f)), color, Math::Vec2(rightUV, bottomUV)),
		MK_VERT(Math::Vec3(rotationMat * Math::Vec4((rPos + Math::Vec2(rect.Size())       ) * scale, 0.f, 1.f)), color, Math::Vec2(rightUV, topUV)),
		MK_VERT(Math::Vec3(rotationMat * Math::Vec4((rPos                                 ) * scale, 0.f, 1.f)), color, Math::Vec2(leftUV,  bottomUV)),
		MK_VERT(Math::Vec3(rotationMat * Math::Vec4((rPos + Math::Vec2(0.f, rect.Height())) * scale, 0.f, 1.f)), color, Math::Vec2(leftUV,  topUV))
	);
	m_previousTexture = tex;	
}

void SpriteBatch::End()
{
	if (VertexAmount()) {
		SubmitBuffer();
		Graphics::SetTexture(&m_previousTexture);
		Graphics::SetTransformMatrix(Math_Identity<Math::Mat4>());
		Graphics::SubmitMesh(this);
	}
	else {
		Graphics::Discard();
	}
}
