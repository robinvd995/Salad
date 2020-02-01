#include "sldpch.h"
#include "Camera.h"

namespace Salad {

	Camera::Camera(CameraProps properties) :
		m_Position(glm::vec3(0)),
		m_Rotation(glm::vec3(0)),
		m_ViewMatrix(glm::mat4()),
		m_ProjectionMatrix(glm::mat4()),
		m_Properties(properties)
	{
		m_ProjectionMatrix = glm::perspective(m_Properties.fov, m_Properties.aspectRatio, 0.1f, 1000.0f);
	}

	Camera::~Camera() {}

	void Camera::calculateViewMatrix() {
		m_ViewMatrix = glm::mat4();
		m_ViewMatrix = m_ViewMatrix * glm::toMat4(m_Rotation);
		m_ViewMatrix = glm::translate(m_ViewMatrix, -m_Position);
	}

}