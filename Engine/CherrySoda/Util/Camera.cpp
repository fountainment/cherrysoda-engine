#include <CherrySoda/Util/Camera.h>

#include <CherrySoda/Engine.h>
#include <CherrySoda/Util/Math.h>

using cherrysoda::Camera;

using cherrysoda::Engine;
using cherrysoda::Math;

// TODO: finish Camera class

Camera::Camera()
{
	m_width = Engine::Instance()->GetWidth();
	m_height = Engine::Instance()->GetHeight();
	UpdateMatrices();
}

Camera::Camera(int width, int height)
{
	m_width = width;
	m_height = height;
	UpdateMatrices();
}

void Camera::UpdateMatrices()
{
	if (m_changed) {
		// TODO: Make use of m_zoom and m_zAngle
		const Math::Vec3 actualPos = m_position - m_origin ;
		m_viewMatrix = Math::LookAt(actualPos, actualPos + m_direction, m_upVector);
		m_projMatrix = Math::Perspective(m_fov, m_width / m_height, 0.1f, 1000.f);
		m_inverseMatrix = Math::InverseMatrix(m_projMatrix * m_viewMatrix);
		m_changed = false;
	}
}