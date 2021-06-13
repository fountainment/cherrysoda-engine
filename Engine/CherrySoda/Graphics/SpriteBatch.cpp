#include <CherrySoda/Graphics/SpriteBatch.h>

#include <CherrySoda/Graphics/Graphics.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/Profile.h>
#include <CherrySoda/Util/STL.h>

using cherrysoda::SpriteBatch;

using cherrysoda::Color;
using cherrysoda::Graphics;
using cherrysoda::Math;
using cherrysoda::SpriteEffects;
using cherrysoda::STL;
using cherrysoda::Texture2D;

namespace type = cherrysoda::type;

void SpriteBatch::Draw(const Texture2D& tex, const Math::Vec2& pos, const Math::IRectangle& rect, \
					const Color& color, float rotation, const Math::Vec2& origin, const Math::Vec2& scale, \
					SpriteEffects effects, float layerDepth)
{
	if (VertexIsNotEmpty()) {
		if (tex.GetHandle() != m_previousTexture.GetHandle()) {
			End(); Begin();
		}
		else {
			type::UInt16 vn = static_cast<type::UInt16>(VertexAmount());
			if (static_cast<type::UInt16>(vn + 4) < vn) {
				End(); Begin();
			}
		}
	}
	float leftUV   = (float)rect.Left()   / (float)tex.Width();
	float rightUV  = (float)rect.Right()  / (float)tex.Width();
	float bottomUV = (float)rect.Top()    / (float)tex.Height();
	float topUV    = (float)rect.Bottom() / (float)tex.Height();
	if ((effects & SpriteEffects::FlipVertically) == SpriteEffects::FlipVertically) {
		STL::Swap(bottomUV, topUV);
	}
	if ((effects & SpriteEffects::FlipHorizontally) == SpriteEffects::FlipHorizontally) {
		STL::Swap(leftUV, rightUV);
	}
	const Math::Vec4 rPos = ((effects & SpriteEffects::RoundRenderingPosition) == SpriteEffects::RoundRenderingPosition) ? Math::Vec4(Math_Round(pos), 0.f, 0.f) : Math::Vec4(pos, 0.f, 0.f);
	Math::Vec2 RBCorner = (Math::Vec2(rect.Width(), 0.f)  - origin) * scale;
	Math::Vec2 RTCorner = (Math::Vec2(rect.Size())        - origin) * scale;
	Math::Vec2 LBCorner = (Vec2_Zero                      - origin) * scale;
	Math::Vec2 LTCorner = (Math::Vec2(0.f, rect.Height()) - origin) * scale;
	if ((effects & SpriteEffects::RoundRenderingPosition) == SpriteEffects::RoundRenderingPosition) {
		RBCorner = Math_Floor(RBCorner);	
		RTCorner = Math_Floor(RTCorner);	
		LBCorner = Math_Floor(LBCorner);	
		LTCorner = Math_Floor(LTCorner);	
	}
	const Math::Mat4 rotationMat = rotation == 0 ? Math_Identity<Math::Mat4>() : Math_Rotate(Math_Identity<Math::Mat4>(), rotation, Vec3_ZUp);
	AddQuad(
		MK_VERT(Math::Vec3(rPos + rotationMat * Math::Vec4(RBCorner, -layerDepth, 1.f)), color, Math::Vec2(rightUV, bottomUV)),
		MK_VERT(Math::Vec3(rPos + rotationMat * Math::Vec4(RTCorner, -layerDepth, 1.f)), color, Math::Vec2(rightUV, topUV   )),
		MK_VERT(Math::Vec3(rPos + rotationMat * Math::Vec4(LBCorner, -layerDepth, 1.f)), color, Math::Vec2(leftUV,  bottomUV)),
		MK_VERT(Math::Vec3(rPos + rotationMat * Math::Vec4(LTCorner, -layerDepth, 1.f)), color, Math::Vec2(leftUV,  topUV   ))
	);
	m_previousTexture = tex;
}

void SpriteBatch::End()
{
	CHERRYSODA_PROFILE_FUNCTION();
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
