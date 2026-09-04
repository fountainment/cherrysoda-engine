#ifndef _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_

#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Entity.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class GraphicsComponent : public Component
{
public:
	CHERRYSODA_DECLARE_COMPONENT(GraphicsComponent, Component);

	GraphicsComponent(bool active) : base(active, true) {}

	CHERRYSODA_GETTER_SETTER_OF_VEC3(Position, m_position);
	CHERRYSODA_GETTER_SETTER_OF_VEC3(Origin, m_origin);
	CHERRYSODA_GETTER_SETTER_OF_VEC3(Scale, m_scale);

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, ZRotation, m_zRotation);

	inline void RotateOnZ(float rotation) { ZRotation(ZRotation() + rotation); }

	// Renders the component at eight offsets in the outline color, then
	// renders it normally on top
	void DrawOutline(const Color& color, int offset = 1)
	{
		Math::Vec3 position = Position();
		Color was = m_color;

		SetColor(color);
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (i != 0 || j != 0) {
					Position(position + Math::Vec3((float)(i * offset), (float)(j * offset), 0.f));
					Render();
				}
			}
		}

		Position(position);
		SetColor(was);
	}
	void DrawOutline(int offset = 1) { DrawOutline(Color::Black, offset); }

	inline void RenderPosition(Math::Vec3 pos3d) { Position(pos3d - EntityPos()); }
	inline const Math::Vec3 RenderPosition() const { return Position() + EntityPos(); }

	inline Color GetColor() const { return m_color; }
	inline void SetColor(const Color& color) { m_color = color; }
	inline void SetColorR(float r) { m_color.R(r); }
	inline void SetColorG(float g) { m_color.G(g); }
	inline void SetColorB(float b) { m_color.B(b); }
	inline void SetColorA(float a) { m_color.A(a); }
	inline void SetColorR(int r) { m_color.R(r); }
	inline void SetColorG(int g) { m_color.G(g); }
	inline void SetColorB(int b) { m_color.B(b); }
	inline void SetColorA(int a) { m_color.A(a); }
	virtual Math::Mat4 GetTransformMatrix() const;

private:
	inline const Math::Vec3 EntityPos() const
	{
		return GetEntity() != nullptr ? GetEntity()->Position() : Math::Vec3(0.f);
	}

	Color m_color = Color::White;
	Math::Vec3 m_position = Math::Vec3(0.f);
	Math::Vec3 m_origin = Math::Vec3(0.f);
	Math::Vec3 m_scale = Math::Vec3(1.f);
	float m_zRotation = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_
