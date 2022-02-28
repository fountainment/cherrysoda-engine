#include <CherrySoda/Graphics/MTexture.h>

#include <CherrySoda/Graphics/SpriteBatch.h>
#include <CherrySoda/Graphics/SpriteEffects.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Draw.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>

using cherrysoda::MTexture;

using cherrysoda::Color;
using cherrysoda::Draw;
using cherrysoda::Math;
using cherrysoda::SpriteEffects;
using cherrysoda::String;
using cherrysoda::StringID;
using cherrysoda::Texture2D;

namespace type = cherrysoda::type;


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
	ActualDrawOffset(IVec2_Zero);
	Width(ClipRect().Width());
	Height(ClipRect().Height());
	SetUtil();
}

MTexture::MTexture(const MTexture& parent, int x, int y, int width, int height)
{
	m_texture = parent.Texture();
	ClipRect(parent.GetRelativeRect(x, y, width, height));
	DrawOffset(Math::Vec2(-Math_Min(x - parent.DrawOffset().x, 0.f), -Math_Min(y - parent.DrawOffset().y, 0.f)));
	ActualDrawOffset(Math::Vec2(DrawOffset().x, -DrawOffset().y));
	Width(ClipRect().Width());
	Height(ClipRect().Height());
	SetUtil();
}

MTexture::MTexture(const MTexture& parent, const Math::IRectangle& clipRect)
: MTexture(parent, clipRect.X(), clipRect.Y(), clipRect.Width(), clipRect.Height())
{
}

MTexture::MTexture(const MTexture& parent, const StringID& atlasPath, const Math::IRectangle& clipRect)
: MTexture(parent, clipRect)
{
	m_atlasPath = atlasPath;
}

MTexture::MTexture(const MTexture& parent, const StringID& atlasPath, const Math::IRectangle& clipRect, const Math::Vec2& drawOffset, int width, int height)
{
	m_texture = parent.Texture();
	m_atlasPath = atlasPath;

	ClipRect(parent.GetRelativeRect(clipRect));
	DrawOffset(drawOffset);
	ActualDrawOffset(Math::Vec2(drawOffset.x, height - clipRect.Height() - drawOffset.y));
	Width(width);
	Height(height);
	SetUtil();
}

MTexture::MTexture(int width, int height, const Color& color)
{
	const size_t pixelCount = static_cast<size_t>(width) * static_cast<size_t>(height);
	unsigned char* data = new unsigned char[pixelCount << 2];
	type::UInt32 colorValue = color.U32ABGR();
	for (size_t i = 0; i < pixelCount; ++i) {
		*((type::UInt32*)(data + (i << 2))) = colorValue;
	}
	m_texture = Texture2D::FromRGBA(data, width, height);
	delete [] data;
	ClipRect({ IVec2_Zero, Math::IVec2(width, height) });
	DrawOffset(IVec2_Zero);
	ActualDrawOffset(IVec2_Zero);
	Width(width);
	Height(height);
	SetUtil();
}

Math::IRectangle MTexture::GetRelativeRect(int x, int y, int width, int height) const
{
	int atX = ClipRect().X() - static_cast<int>(DrawOffset().x) + x;
	int atY = ClipRect().Y() - static_cast<int>(DrawOffset().y) + y;

	int rX = Math_Clamp(atX, ClipRect().Left(), ClipRect().Right());
	int rY = Math_Clamp(atY, ClipRect().Bottom(), ClipRect().Top());
	int rW = Math_Max(0, Math_Min(atX + width, ClipRect().Right()) - rX);
	int rH = Math_Max(0, Math_Min(atY + height, ClipRect().Top()) - rY);

	return { Math::IVec2(rX, rY), Math::IVec2(rW, rH) };
}

Math::IRectangle MTexture::GetRelativeRect(const Math::IRectangle& rect) const
{
	return GetRelativeRect(rect.X(), rect.Y(), rect.Width(), rect.Height());
}

MTexture MTexture::GetSubtexture(int x, int y, int width, int height) const
{
	return MTexture(*this, x, y, width, height);
}

MTexture MTexture::GetSubtexture(const Math::IRectangle& rect) const
{
	return MTexture(*this, rect);
}

void MTexture::Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin/* = Vec3_Zero*/, const Color& color/* = Color::White*/, const Math::Vec3& scale/* = Vec3_One*/, float zRotation/* = 0.f*/, SpriteEffects flip/* = SpriteEffects::None*/) const
{
	Draw::GetSpriteBatch()->Draw(Texture(), Math::Vec2(renderPosition), ClipRect(), color, zRotation, Math::Vec2(origin) - ActualDrawOffset(), Math::Vec2(scale), flip, 0.f);
}

void MTexture::SetUtil()
{
	m_center = Math::Vec2(Width(), Height()) * 0.5f;
	m_leftUV = ClipRect().Left() / (float)Texture().Width();
	m_rightUV = ClipRect().Right() / (float)Texture().Width();
	m_bottomUV = ClipRect().Top() / (float)Texture().Height();
	m_topUV = ClipRect().Bottom() / (float)Texture().Height();
}
