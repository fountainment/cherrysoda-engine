#ifndef _CHERRYSODA_UTIL_CAMERA_H_
#define _CHERRYSODA_UTIL_CAMERA_H_

#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera
{
public:
	Camera();
	Camera(int width, int height);

	inline cherrysoda::Math::Vec2 GetSize() { return cherrysoda::Math::Vec2(m_width, m_height); }
	inline void SetSize(const cherrysoda::Math::Vec2& size) { m_width = size[0]; m_height = size[1]; }

	void UpdateMatrices();

	inline Math::Mat4* GetViewMatrix() { UpdateMatrices(); return &m_viewMatrix; }
	inline Math::Mat4* GetProjectionMatrix() { UpdateMatrices(); return &m_projMatrix; }

	inline const Math::Vec3 GetLeftVector() { return Math_Normalize(Math_Cross(m_upVector, m_direction)); }
	inline const Math::Vec3 GetRightVector() { return Math_Normalize(Math_Cross(m_direction, m_upVector)); }

	inline const Math::Vec3 GetUpVector() { return m_upVector; }
	inline const Math::Vec3 GetDownVector() { return -m_upVector; }

	inline const Math::Vec3 GetFrontVector() { return m_direction; }
	inline const Math::Vec3 GetBackVector() { return -m_direction; }

	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Position, m_position, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Scale, m_zoom, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Origin, m_origin, CHERRYSODA_NONE_OP, m_changed = true);
	CHERRYSODA_MATH_VEC3_GETTER_SETTER_EX(Direction, m_direction, CHERRYSODA_NONE_OP, m_changed = true);

	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, ZRotation, m_zAngle, CHERRYSODA_NONE_OP, if (m_zAngle == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, Width, m_width, CHERRYSODA_NONE_OP, if (m_width == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, Height, m_height, CHERRYSODA_NONE_OP, if (m_height == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, FOV, m_fov, CHERRYSODA_NONE_OP, if (m_fov == v) return; m_changed = true);

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
	Math::Vec3 m_direction = -Vec3_ZUp;
	float m_zAngle = 0.f;
	Math::Vec3 m_upVector = Vec3_YUp;
	float m_fov = 60.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CAMERA_H_