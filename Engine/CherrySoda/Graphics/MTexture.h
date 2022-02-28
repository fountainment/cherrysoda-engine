#ifndef _CHERRYSODA_GRAPHICS_MTEXTURE_H_
#define _CHERRYSODA_GRAPHICS_MTEXTURE_H_

#include <CherrySoda/Graphics/SpriteEffects.h>
#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>


namespace cherrysoda {

class MTexture
{
public:
	static MTexture FromFile(const String& filename);

	MTexture() = default;
	MTexture(const Texture2D& texture);
	MTexture(const MTexture& parent, int x, int y, int width, int height);
	MTexture(const MTexture& parent, const Math::IRectangle& clipRect);
	MTexture(const MTexture& parent, const StringID& atlasPath, const Math::IRectangle& clipRect);
	MTexture(const MTexture& parent, const StringID& atlasPath, const Math::IRectangle& clipRect, const Math::Vec2& drawOffset, int width, int height);
	MTexture(int width, int height, const Color& color);

	Math::IRectangle GetRelativeRect(int x, int y, int width, int height) const;
	Math::IRectangle GetRelativeRect(const Math::IRectangle& rect) const;

	MTexture GetSubtexture(int x, int y, int width, int height) const;
	MTexture GetSubtexture(const Math::IRectangle& rect) const;

	const Math::IRectangle& ClipRect() const { return m_clipRect; }

	void Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin = Vec3_Zero, const Color& color = Color::White, const Math::Vec3& scale = Vec3_One, float zRotation = 0.f, SpriteEffects flip = SpriteEffects::None) const;

	inline Math::Vec2 DrawOffset() const { return m_drawOffset; }
	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }
	inline Math::Vec2 Center() const { return m_center; }
	inline Math::IVec2 Size() const { return Math::IVec2(m_width, m_height); }

	void SetUtil();

	inline bool IsValid() const { return Texture().IsValid(); }

	inline const Texture2D& Texture() const { return m_texture; }

	bool operator == (const MTexture& texture) const
	{
		if (!m_atlasPath.IsEmpty() || !texture.m_atlasPath.IsEmpty()) {
			return m_atlasPath == texture.m_atlasPath;
		}
		if (m_texture != texture.m_texture)
			return false;
		if (m_clipRect != texture.m_clipRect)
			return false;
		if (m_actualDrawOffset != texture.m_actualDrawOffset)
			return false;
		if (m_width != texture.m_width || m_height != texture.m_height)
			return false;
		return true;
	}

private:
	inline Math::Vec2 ActualDrawOffset() const { return m_actualDrawOffset; }

	inline void ClipRect(const Math::IRectangle& rect) { m_clipRect = rect; }
	inline void DrawOffset(const Math::Vec2& drawOffset) { m_drawOffset = drawOffset; }
	inline void ActualDrawOffset(const Math::Vec2& actualDrawOffset) { m_actualDrawOffset = actualDrawOffset; }
	inline void Width(int width) { m_width = width; }
	inline void Height(int height) { m_height = height; }

	Texture2D m_texture;
	Math::IRectangle m_clipRect = { IVec2_Zero, IVec2_Zero };
	StringID m_atlasPath;
	Math::Vec2 m_drawOffset = Vec2_Zero;
	Math::Vec2 m_actualDrawOffset = Vec2_Zero;
	int m_width = 0;
	int m_height = 0;
	Math::Vec2 m_center = Vec2_Zero;
	float m_leftUV = 0.f;
	float m_rightUV = 0.f;
	float m_topUV = 0.f;
	float m_bottomUV = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_MTEXTURE_H_
