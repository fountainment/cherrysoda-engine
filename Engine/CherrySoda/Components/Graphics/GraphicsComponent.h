#ifndef _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_
#define _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_

#include <CherrySoda/Entity.h>
#include <CherrySoda/Components/Component.h>
#include <CherrySoda/Util/Color.h>
#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class GraphicsComponent : public Component
{
public:
	inline void Position(Math::Vec3 pos3d) { m_position = pos3d; }
	inline void Position(Math::Vec2 pos2d) { Position(Math::Vec3(pos2d, Position()[2])); }
	inline const Math::Vec3 Position() const { return m_position; }

	inline void RenderPosition(Math::Vec3 pos3d) { Position(pos3d - EntityPos()); }
	inline const Math::Vec3 RenderPosition() const { return Position() + EntityPos(); }

private:
	inline const Math::Vec3 EntityPos() const { return GetEntity() != nullptr ? GetEntity()->Position() : Math::Vec3(0.f); }

	Color m_color = Color::White;
	Math::Vec3 m_position = Math::Vec3(0.f);
	Math::Vec3 m_origin = Math::Vec3(0.f);
	Math::Vec3 m_scale = Math::Vec3(1.f);
	float m_zRotation = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COMPONENTS_GRAPHICSCOMPONENT_H_