#ifndef _CHERRYSODA_UTIL_CAMERA_H_
#define _CHERRYSODA_UTIL_CAMERA_H_

#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera
{
public:
	Camera();
	Camera(int width, int height);

	void UpdateMatrices();

	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Position, m_position, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Scale, m_zoom, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Origin, m_origin, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Direction, m_direction, CHERRYSODA_NONE_OP, m_changed = true);

private:
	friend class Graphics;

	Math::Mat4 m_viewMatrix;
	Math::Mat4 m_projMatrix;
	Math::Mat4 m_inverseMatrix;
	bool m_changed = true;

	float m_width;
	float m_height;
	Math::Vec3 m_position = Vec3_Zero;
	Math::Vec3 m_zoom = Vec3_One;
	Math::Vec3 m_origin = Vec3_Zero;
	Math::Vec3 m_direction = Vec3_ZUp;
	float m_zAngle = 0.f;
	Math::Vec3 m_upVector = Vec3_YUp;
	float m_fov = 60.f;

public:
	CHERRYSODA_GETTER_SETTER(ZRotation, m_zAngle);
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CAMERA_H_