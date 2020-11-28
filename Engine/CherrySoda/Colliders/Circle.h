#ifndef _CHERRYSODA_COLLIDERS_CIRCLE_H_
#define _CHERRYSODA_COLLIDERS_CIRCLE_H_

#include <CherrySoda/Colliders/Collider.h>

namespace cherrysoda {

class Camera;
class Color;

class Circle : public Collider
{
public:
	Circle(float radius, const Math::Vec2& center = Vec2_Zero)
	{
		Radius(radius);
		Position2D(center);
	}

	bool Collide(const Circle* circle) const override;
	bool Collide(const Math::Vec2& point) const override;
	void Render(const Camera* camera, const Color& color) const override;

	inline float RadiusSq() const { return m_radius * m_radius; }
	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Radius, m_radius);

private:
	float m_radius = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_CIRCLE_H_
