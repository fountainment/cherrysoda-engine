#ifndef _CHERRYSODA_UTIL_CAMERA_H_
#define _CHERRYSODA_UTIL_CAMERA_H_

#include <CherrySoda/Util/Math.h>

namespace cherrysoda {

class Camera
{
public:
	Camera();
	Camera(int width, int height);

private:
	friend class Graphics;

	Math::Mat4 m_viewMatrix;
	Math::Mat4 m_projMatrix;
	Math::Mat4 m_inverse;
	bool m_changed = false;

	Math::Vec3 m_position = Math::Vec3(0.f);
	Math::Vec2 m_zoom = Math::Vec2(1.f);
	Math::Vec3 m_origin = Math::Vec3(0.f);
	float m_zAngle = 0.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CAMERA_H_