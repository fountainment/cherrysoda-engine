#include <CherrySoda/Graphics/MTexture.h>

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::MTexture;

using cherrysoda::Color;
using cherrysoda::Math;
using cherrysoda::String;
using cherrysoda::Texture2D;

MTexture MTexture::FromFile(const String& filename)
{
	Texture2D result = Texture2D::FromFile(filename);
	return MTexture(result);
}

MTexture::MTexture(const Texture2D& texture)
{
	m_texture = texture;
	ClipRect({ IVec2_Zero, Math::IVec2(texture.Width(), texture.Height()) });
	DrawOffset(IVec2_Zero);
	Width(ClipRect().Width());
	Height(ClipRect().Height());
	SetUtil();
}

MTexture::MTexture(const MTexture& parent, int x, int y, int width, int height)
{
	m_texture = parent.Texture();
	ClipRect(parent.GetRelativeRect(x, y, width, height));
	DrawOffset(Math::IVec2(-Math_Min(x - parent.DrawOffset().x, 0), -Math_Min(y - parent.DrawOffset().y, 0)));
}

MTexture::MTexture(const MTexture& parent, const Math::IRectangle& clipRect)
: MTexture(parent, clipRect.X(), clipRect.Y(), clipRect.Width(), clipRect.Height())
{
}

Math::IRectangle MTexture::GetRelativeRect(int x, int y, int width, int height) const
{
	int atX = ClipRect().X() - DrawOffset().x + x;
	int atY = ClipRect().Y() - DrawOffset().y + y;

	int rx = Math_Clamp(atX, ClipRect().Left(), ClipRect().Right());
	return {  };
}

Math::IRectangle MTexture::GetRelativeRect(const Math::IRectangle& rect) const
{
	return GetRelativeRect(rect.X(), rect.Y(), rect.Width(), rect.Height());
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

void MTexture::SetUtil()
{
}
