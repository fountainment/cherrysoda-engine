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
		m_viewMatrix = Math::ScaleMat4(Math::LookAt(actualPos, actualPos + m_direction, Math::RotateVector(m_upVector, ZRotation(), Vec3_ZUp)), m_zoom);
		m_projMatrix = Math::Perspective(glm::radians(m_fov), m_width / m_height, 0.1f, 1000.f);
		m_inverseMatrix = Math::InverseMat4(m_projMatrix * m_viewMatrix);
		m_changed = false;
	}
}