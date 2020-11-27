#ifndef _CHERRYSODA_COLLIDERS_CIRCLE_H_
#define _CHERRYSODA_COLLIDERS_CIRCLE_H_

#include <CherrySoda/Colliders/Collider.h>

namespace cherrysoda {

class Camera;
class Color;

class Circle : public Collider
{
public:
	Circle(float radius, const Math::Vec2& center)
	{
		Radius(radius);
		Position2D(center);
	}

	bool Collide(const Circle* circle) const override;
	void Render(const Camera* camera, const Color& color) const override;

	CHERRYSODA_GETTER_SETTER_OF_TYPE(float, Radius, m_radius);

private:
	float m_radius = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_COLLIDERS_CIRCLE_H_
