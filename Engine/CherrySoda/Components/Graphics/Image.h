#ifndef _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_

#include <CherrySoda/Graphics/GraphicsComponent.h>
#include <CherrySoda/Graphics/MTexture.h>

// TODO: Use shared_ptr for texture?

namespace cherrysoda {

class Image : public GraphicsComponent
{
public:
	Image(const MTexture& texture)
	: base(false)
	, m_texture(texture)
	{
	}

	Image(const MTexture& texture, bool active)
	: base(active)
	, m_texture(texture)
	{
	}

	void Render() override
	{
		m_texture.Draw(RenderPosition(), Origin(), GetColor(), Scale(), ZRotation());
	}

	inline float Width() const { return m_texture.Width(); }
	inline float Height() const { return m_texture.Height(); }
	inline const  Math::Vec2 Size() const { return Math::Vec2(Width(), Height()); } 
	inline const Math::Vec2 HalfSize() const { return Size() * 0.5f; }

	Image& CenterOrigin()
	{
		Origin(HalfSize());
		return *this;
	}

	Image& JustifyOrigin(const Math::Vec2& at)
	{
		Origin(Size() * at);
		return *this;
	}

private:
	MTexture m_texture;
};

}; // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICS_IMAGE_H_
