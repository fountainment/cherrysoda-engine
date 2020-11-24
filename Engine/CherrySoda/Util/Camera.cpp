#include <CherrySoda/Util/Camera.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Camera;

using cherrysoda::Engine;
using cherrysoda::Math;

// TODO: finish Camera class

Camera::Camera()
{
	SetSize(Engine::Instance()->GetViewSize());
	UpdateMatrices();
}

Camera::Camera(int width, int height)
{
	SetSize(Math::IVec2(width, height));
	UpdateMatrices();
}

void Camera::UpdateMatrices()
{
	if (m_changed) {
		const Math::Vec3 actualPos = m_position - m_origin;
		m_viewMatrix = Math_Scale(Math_LookAt(actualPos, actualPos + m_direction, Math_RotateVector(m_upVector, ZRotation(), Vec3_ZUp)), m_zoom);
		m_projMatrix = Math_Perspective(Math_Radians(m_fov), Ratio(), 0.1f, 1000.f);
		m_inverseMatrix = Math_Inverse(m_projMatrix * m_viewMatrix);
		m_changed = false;
	}
}

void Camera::Approach(const Math::Vec2& pos, float ease)
{
	Approach(Math::Vec3(pos, Position().z), ease);
}

void Camera::Approach(const Math::Vec2& pos, float ease, float maxDistance)
{
	Approach(Math::Vec3(pos, Position().z), ease, maxDistance);
}

void Camera::Approach(const Math::Vec3& pos, float ease)
{
	Position(Position() + (pos - Position()) * ease);
}

void Camera::Approach(const Math::Vec3& pos, float ease, float maxDistance)
{

	Math::Vec3 move = (pos - Position()) * ease;
	if (Math_Length(move) > maxDistance) {
		Position(Position() + Math_Normalize(move) * maxDistance);
	}
	else {
		Position(Position() + move);	
	}
}
