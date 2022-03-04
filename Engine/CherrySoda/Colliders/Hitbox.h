#ifndef _CHERRYSODA_COLLIDERS_HITBOX_H_
#define _CHERRYSODA_COLLIDERS_HITBOX_H_

#include <CherrySoda/Colliders/Collider.h>

namespace cherrysoda {

class Camera;
class Circle;
class ColliderList;
class Color;
class Grid;

class Hitbox : public Collider
{
public:
	CHERRYSODA_DECLARE_COLLIDER(Hitbox, Collider);

	Hitbox(float width, float height, float x = 0.f, float y = 0.f)
		: m_width(width)
		, m_height(height)
	{
		Position2D(Math::Vec2(x, y));
	}

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

	float Width() const override { return m_width; }
	float Height() const override { return m_height; }
	void Width(float width) { m_width = width; }
	void Height(float height) { m_height = height; }

	inline float AbsoluteLeft() const { return GetEntity() ? InternalLeft() + GetEntity()->PositionX() : InternalLeft(); }
	inline float AbsoluteRight() const { return GetEntity() ? InternalRight() + GetEntity()->PositionX() : InternalRight(); }
	inline float AbsoluteBottom() const { return GetEntity() ? InternalBottom() + GetEntity()->PositionY() : InternalBottom(); }
	inline float AbsoluteTop() const { return GetEntity() ? InternalTop() + GetEntity()->PositionY() : InternalTop(); }

	void CenterOrigin()
	{
		PositionX(-m_width / 2.f);
		PositionY(-m_height / 2.f);
	}

	inline bool Intersects(const Hitbox* hitbox) const
	{
		return AbsoluteLeft() < hitbox->AbsoluteRight() && AbsoluteRight() > hitbox->AbsoluteLeft() &&
			AbsoluteBottom() < hitbox->AbsoluteTop() && AbsoluteTop() > hitbox->AbsoluteBottom();
	}

private:
	inline float InternalLeft() const { return PositionX(); }
	inline float InternalRight() const { return PositionX() + m_width; }
	inline float InternalBottom() const { return PositionY(); }
	inline float InternalTop() const { return PositionY() + m_height; }

	float m_width;
	float m_height;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_HITBOX_H_
