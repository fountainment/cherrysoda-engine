#include <CherrySoda/Graphics/MTexture.h>

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::MTexture;

using cherrysoda::Math;

MTexture MTexture::FromFile(const String& filename)
{
	Texture2D result = Texture2D::FromFile(filename);
	return MTexture(result);
}

MTexture::MTexture(const Texture2D& texture2D)
{
	m_texture = texture2D;
	ClipRect({ IVec2_Zero, Math::IVec2(texture2D.Width(), texture2D.Height()) });
}

MTexture::MTexture(const Texture2D& texture2D, const Math::IRectangle& rect)
{
	m_texture = texture2D;
	ClipRect(rect);
}

Math::IRectangle MTexture::GetRelativeRect(const Math::IRectangle& rect)
{
	Math::IRectangle result = { ClipRect()->m_coord + rect.m_coord, rect.m_size };
	// TODO: Clamp rect size
	return result;
}

MTexture MTexture::GetSubtexture(const Math::IRectangle& rect)
{
	auto r = GetRelativeRect(rect);
	return MTexture(m_texture, r);
}

void MTexture::Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin, const Color& color, const Math::Vec3& scale, float zRotation)
{
	// TODO: Draw::GetSpriteBatch()->Draw();
}
