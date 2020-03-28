#include <CherrySoda/Graphics/MTexture.h>

#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::MTexture;

using cherrysoda::Color;
using cherrysoda::Math;
using cherrysoda::SpriteEffects;
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
	DrawOffset(Math::Vec2(-Math_Min(x - parent.DrawOffset().x, 0.f), -Math_Min(y - parent.DrawOffset().y, 0.f)));
	Width(ClipRect().Width());
	Height(ClipRect().Height());
	SetUtil();
}

MTexture::MTexture(const MTexture& parent, const Math::IRectangle& clipRect)
: MTexture(parent, clipRect.X(), clipRect.Y(), clipRect.Width(), clipRect.Height())
{
}

Math::IRectangle MTexture::GetRelativeRect(int x, int y, int width, int height) const
{
	int atX = ClipRect().X() - static_cast<int>(DrawOffset().x) + x;
	int atY = ClipRect().Y() - static_cast<int>(DrawOffset().y) + y;

	int rX = Math_Clamp(atX, ClipRect().Left(), ClipRect().Right());
	int rY = Math_Clamp(atY, ClipRect().Bottom(), ClipRect().Top());
	int rW = Math_Max(0, Math_Min(atX + Width(), ClipRect().Right()) - rX);
	int rH = Math_Max(0, Math_Min(atY + Height(), ClipRect().Top()) - rY);

	return { Math::IVec2(rX, rY), Math::IVec2(rW, rH) };
}

Math::IRectangle MTexture::GetRelativeRect(const Math::IRectangle& rect) const
{
	return GetRelativeRect(rect.X(), rect.Y(), rect.Width(), rect.Height());
}

MTexture MTexture::GetSubtexture(const Math::IRectangle& rect)
{
	return MTexture(Texture(), rect);
}

void MTexture::Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin, const Color& color, const Math::Vec3& scale, float zRotation, SpriteEffects flip)
{
	Draw::GetSpriteBatch()->Draw(Texture(), Math::Vec2(renderPosition), ClipRect(), color, zRotation, Math::Vec2(origin) - DrawOffset(), Math::Vec2(scale), flip, 0.f);
}

void MTexture::SetUtil()
{
	m_center = Math::Vec2(Width(), Height()) * 0.5f;
	m_leftUV = ClipRect().Left() / (float)Texture().Width();
	m_rightUV = ClipRect().Right() / (float)Texture().Width();
	m_bottomUV = ClipRect().Bottom() / (float)Texture().Height();
	m_topUV = ClipRect().Top() / (float)Texture().Height();
}
