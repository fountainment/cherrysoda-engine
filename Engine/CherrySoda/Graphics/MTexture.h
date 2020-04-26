#ifndef _CHERRYSODA_GRAPHICS_MTEXTURE_H_
#define _CHERRYSODA_GRAPHICS_MTEXTURE_H_

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/NumType.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Draw.h>

namespace cherrysoda {

enum class SpriteEffects : type::UInt8;

class MTexture
{
public:
	static MTexture FromFile(const String& filename);

	MTexture() {}
	MTexture(const Texture2D& texture);
	MTexture(const MTexture& parent, int x, int y, int width, int height);
	MTexture(const MTexture& parent, const Math::IRectangle& clipRect);
	MTexture(const MTexture& parent, const String& atlasPath, const Math::IRectangle& clipRect);

	Math::IRectangle GetRelativeRect(int x, int y, int width, int height) const;
	Math::IRectangle GetRelativeRect(const Math::IRectangle& rect) const;

	MTexture GetSubtexture(const Math::IRectangle& rect);

	const Math::IRectangle& ClipRect() const { return m_clipRect; }

	void Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin, const Color& color, const Math::Vec3& scale, float zRotation, SpriteEffects flip);

	inline Math::Vec2 DrawOffset() const { return m_drawOffset; }
	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }

	void SetUtil();

	inline bool IsValid() const { return Texture().IsValid(); }

private:
	inline void ClipRect(const Math::IRectangle& rect) { m_clipRect = rect; }
	inline void DrawOffset(const Math::Vec2& drawOffset) { m_drawOffset = drawOffset; }
	inline void Width(int width) { m_width = width; }
	inline void Height(int height) { m_height = height; }
	inline const Texture2D& Texture() const { return m_texture; }

	Texture2D m_texture;
	Math::IRectangle m_clipRect = { IVec2_Zero, IVec2_Zero };
	String m_atlasPath = "";
	Math::Vec2 m_drawOffset = Vec2_Zero;
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
