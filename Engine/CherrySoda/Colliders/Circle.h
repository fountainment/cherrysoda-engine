#ifndef _CHERRYSODA_COLLIDERS_CIRCLE_H_
#define _CHERRYSODA_COLLIDERS_CIRCLE_H_

#include <CherrySoda/Colliders/Collider.h>

namespace cherrysoda {

class Camera;
class ColliderList;
class Color;
class Grid;
class Hitbox;

class Circle : public Collider
{
public:
	CHERRYSODA_DECLARE_COLLIDER(Circle, Collider);

	Circle(float radius, const Math::Vec2& center = Vec2_Zero)
	{
		Radius(radius);
		Position2D(center);
	}

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Radius, m_radius);

	bool Collide(const Circle* circle) const override;
	bool Collide(const ColliderList* list) const override;
	bool Collide(const Hitbox* hitbox) const override;
	bool Collide(const Grid* grid) const override;
	bool Collide(const Math::Vec2& point) const override;
	bool Collide(const Math::Rectangle& rect) const override;
	bool Collide(const Math::Vec2& from, const Math::Vec2& to) const override;
	void Render(const Camera* camera, const Color& color) const override;

	float Left() const override { return InternalLeft(); }
	float Right() const override { return InternalRight(); }
	float Bottom() const override { return InternalBottom(); }
	float Top() const override { return InternalTop(); }

	float Width() const override { return Radius() * 2.f; }
	float Height() const override { return Radius() * 2.f; }

	inline float RadiusSq() const { return m_radius * m_radius; }

private:
	inline float InternalLeft() const { return PositionX() - Radius(); }
	inline float InternalRight() const { return PositionX() + Radius(); }
	inline float InternalBottom() const { return PositionY() - Radius(); }
	inline float InternalTop() const { return PositionY() + Radius(); }

	float m_radius = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_CIRCLE_H_
