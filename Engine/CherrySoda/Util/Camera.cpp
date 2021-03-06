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
		if (UseOrthoProjection()) {
			m_viewMatrix = Math_Translate(Math_Scale(Math_Rotate(Math_Translate(Math_Identity<Math::Mat4>(), Origin()), -ZRotation(), Vec3_ZUp), Scale()), -Position());
			m_projMatrix = Math_Ortho(0.f, m_width, 0.f, m_height, -10000.f, 10000.f);
		}
		else {
			const Math::Vec3 actualPos = Position() - Origin();
			m_viewMatrix = Math_LookAt(actualPos, actualPos + Direction(), Math_RotateVector(GetUpVector(), ZRotation(), Vec3_ZUp));
			m_projMatrix = Math_Scale(Math_Perspective(Math_Radians(FOV()), Ratio(), 0.1f, 10000.f), m_zoom);
		}
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
