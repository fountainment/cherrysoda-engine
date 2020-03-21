#ifndef _CHERRYSODA_GRAPHICS_MTEXTURE_H_
#define _CHERRYSODA_GRAPHICS_MTEXTURE_H_

#include <CherrySoda/Graphics/Texture.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>
#include <CherrySoda/Util/Draw.h>

namespace cherrysoda {

class MTexture
{
public:
	static MTexture FromFile(const String& filename);

	MTexture() {}
	MTexture(const Texture2D& texture);
	MTexture(const MTexture& parent, int x, int y, int width, int height);
	MTexture(const MTexture& parent, const Math::IRectangle& clipRect);

	Math::IRectangle GetRelativeRect(int x, int y, int width, int height) const;
	Math::IRectangle GetRelativeRect(const Math::IRectangle& rect) const;

	MTexture GetSubtexture(const Math::IRectangle& rect);

	const Math::IRectangle& ClipRect() const { return m_clipRect; }

	void Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin, const Color& color, const Math::Vec3& scale, float zRotation);

	inline Math::IVec2 DrawOffset() const { return m_drawOffset; }
	inline int Width() const { return m_width; }
	inline int Height() const { return m_height; }

	void SetUtil();

private:
	inline void ClipRect(const Math::IRectangle& rect) { m_clipRect = rect; }
	inline void DrawOffset(const Math::IVec2& drawOffset) { m_drawOffset = drawOffset; }
	inline void Width(int width) { m_width = width; }
	inline void Height(int height) { m_height = height; }
	inline Texture2D Texture() const { return m_texture; }

	Texture2D m_texture;
	Math::IRectangle m_clipRect;
	String m_atlasPath;
	Math::IVec2 m_drawOffset;
	int m_width;
	int m_height;
	Math::Vec2 m_center;
	float m_leftUV;
	float m_rightUV;
	float m_topUV;
	float m_bottomUV;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_GRAPHICS_MTEXTURE_H_
