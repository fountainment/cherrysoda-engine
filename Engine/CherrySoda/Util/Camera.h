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

private:
	friend class Graphics;

	Math::Mat4 m_viewMatrix;
	Math::Mat4 m_projMatrix;
	Math::Mat4 m_inverseMatrix;
	bool m_changed = true;

	float m_width;
	float m_height;
	Math::Vec3 m_position = Math::Vec3(0.f);
	Math::Vec2 m_zoom = Math::Vec2(1.f);
	Math::Vec3 m_origin = Math::Vec3(0.f);
	Math::Vec3 m_direction = Math::Vec3(0.f, 0.f, 1.f);
	float m_zAngle = 0.f;
	Math::Vec3 m_upVector = Math::Vec3(0.f, 1.f, 0.f);
	float m_fov = 60.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CAMERA_H_