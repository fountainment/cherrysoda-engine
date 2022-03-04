#ifndef _CHERRYSODA_UTIL_CAMERA_H_
#define _CHERRYSODA_UTIL_CAMERA_H_

#include <CherrySoda/Util/Log.h>
#include <CherrySoda/Util/Math.h>
#include <CherrySoda/Util/String.h>


namespace cherrysoda {

class Camera
{
public:
	Camera();
	Camera(int width, int height);

	CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(Position, m_position, CHERRYSODA_NONE_OP, if (m_position == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(Scale, m_zoom, CHERRYSODA_NONE_OP, if (m_zoom == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(Origin, m_origin, CHERRYSODA_NONE_OP, if (m_origin == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_VEC3(Direction, m_direction, CHERRYSODA_NONE_OP, if (m_direction == v) return; m_changed = true);

	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, ZRotation, m_zAngle, CHERRYSODA_NONE_OP, if (m_zAngle == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, Width, m_width, CHERRYSODA_NONE_OP, if (m_width == v) return; Ratio(v / Height()); m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, Height, m_height, CHERRYSODA_NONE_OP, if (m_height == v) return; Ratio(Width() / v); m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, FOV, m_fov, CHERRYSODA_NONE_OP, if (m_fov == v) return; m_changed = true);

	CHERRYSODA_GETTER_SETTER_EX_OF_TYPE(float, Ratio, m_ratio, CHERRYSODA_NONE_OP, if(m_ratio == v) return; m_changed = true);
	CHERRYSODA_GETTER_SETTER_EX_OF_BOOL(UseOrthoProjection, m_orthoProjection, CHERRYSODA_NONE_OP, if(m_orthoProjection == v) return; m_changed = true);

	inline Math::Vec2 GetSize() const { return Math::Vec2(m_width, m_height); }
	inline void SetSize(const Math::Vec2& size) { Width(size[0]); Height(size[1]); Ratio(Width() / Height()); }
	inline Math::Vec2 Size() const { return GetSize(); }
	inline void Size(const Math::Vec2& size) { SetSize(size); }

	void UpdateMatrices();

	inline void SetUpVector(const Math::Vec3& vec) { if (m_upVector != vec) { m_upVector = vec; m_changed = true; } }

	inline const Math::Mat4* GetViewMatrix() { if (m_changed) { UpdateMatrices(); } return &m_viewMatrix; }
	inline const Math::Mat4* GetProjectionMatrix() { if (m_changed) { UpdateMatrices(); } return &m_projMatrix; }

	inline const Math::Vec3 GetLeftVector() const { return Math_Normalize(Math_Cross(m_upVector, m_direction)); }
	inline const Math::Vec3 GetRightVector() const { return Math_Normalize(Math_Cross(m_direction, m_upVector)); }

	inline const Math::Vec3 GetUpVector() const { return m_upVector; }
	inline const Math::Vec3 GetDownVector() const { return -m_upVector; }

	inline const Math::Vec3 GetFrontVector() const { return m_direction; }
	inline const Math::Vec3 GetBackVector() const { return -m_direction; }

	inline float Left()   const { return (m_inverseMatrix * Math::Vec4(-Vec3_XUp, 1.f)).x; }
	inline float Right()  const { return (m_inverseMatrix * Math::Vec4( Vec3_XUp, 1.f)).x; }
	inline float Bottom() const { return (m_inverseMatrix * Math::Vec4(-Vec3_YUp, 1.f)).y; }
	inline float Top()    const { return (m_inverseMatrix * Math::Vec4( Vec3_YUp, 1.f)).y; }

	inline float Left()   { if (m_changed) { UpdateMatrices(); } return (m_inverseMatrix * Math::Vec4(-Vec3_XUp, 1.f)).x; }
	inline float Right()  { if (m_changed) { UpdateMatrices(); } return (m_inverseMatrix * Math::Vec4( Vec3_XUp, 1.f)).x; }
	inline float Bottom() { if (m_changed) { UpdateMatrices(); } return (m_inverseMatrix * Math::Vec4(-Vec3_YUp, 1.f)).y; }
	inline float Top()    { if (m_changed) { UpdateMatrices(); } return (m_inverseMatrix * Math::Vec4( Vec3_YUp, 1.f)).y; }

	inline void CenterOrigin()
	{
		CHERRYSODA_ASSERT(UseOrthoProjection(), "Shouldn't call CenterOrigin() on perspective camera.\n");
		Origin(Math_Floor(GetSize() * 0.5f));
	}
	inline void RoundPosition() { Position(Math_Round(Position())); }

	void Approach(const Math::Vec2& pos, float ease);
	void Approach(const Math::Vec2& pos, float ease, float maxDistance);
	void Approach(const Math::Vec3& pos, float ease);
	void Approach(const Math::Vec3& pos, float ease, float maxDistance);
	void Approach(const Math::Vec2& pos, const Math::Rectangle& rect, float ease);

	inline Math::Vec2 ScreenToWorld(const Math::Vec2& screenPos) const
	{
		CHERRYSODA_ASSERT(UseOrthoProjection(), "Camera::ScreenToWorld is for ortho projection camera.\n");
		return Position2D() + (screenPos - Origin2D()) / Scale2D();
	}

private:
	friend class Graphics;

	Math::Mat4 m_viewMatrix;
	Math::Mat4 m_projMatrix;
	Math::Mat4 m_inverseMatrix;
	bool m_changed = true;
	bool m_orthoProjection = false;

	float m_width = 1.f;
	float m_height = 1.f;
	float m_ratio = 1.f;
	Math::Vec3 m_position = Math::Vec3(0.f, 0.f, 1.f);
	Math::Vec3 m_zoom = Vec3_One;
	Math::Vec3 m_origin = Vec3_Zero;
	Math::Vec3 m_direction = -Vec3_ZUp;
	float m_zAngle = 0.f;
	Math::Vec3 m_upVector = Vec3_YUp;
	float m_fov = 60.f;
};

} // namespace cherrysoda

#endif // _CHERRYSODA_UTIL_CAMERA_H_
