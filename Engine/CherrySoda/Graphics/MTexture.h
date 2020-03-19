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

	MTexture(const Texture2D& texture2D);
	MTexture(const Texture2D& texture2D, const Math::IRectangle& rect);

	Math::IRectangle GetRelativeRect(const Math::IRectangle& rect);

	MTexture GetSubtexture(const Math::IRectangle& rect);

	const Math::IRectangle* ClipRect() { return &m_clipRect; }

	void Draw(const Math::Vec3& renderPosition, const Math::Vec3& origin, const Color& color, const Math::Vec3& scale, float zRotation);

private:
	inline void ClipRect(const Math::IRectangle& rect) { m_clipRect = rect; }

	Texture2D m_texture;
	Math::IRectangle m_clipRect;
	String m_atlasPath;
	Math::Vec2 m_drawOffset;
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
