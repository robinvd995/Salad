#include "sldpch.h"
#include "Camera.h"

namespace Salad {

	PerspectiveCamera::PerspectiveCamera(PerspectiveCameraProps properties) :
		m_Properties(properties),
		m_ProjectionMatrix(glm::perspective(glm::radians(properties.fov), properties.aspectRatio, properties.nearPlane, properties.farPlane)),
		m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	PerspectiveCamera::~PerspectiveCamera() {}

	void PerspectiveCamera::recalculateViewProjectionMatrix() {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top) :
		m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f) {
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top) {
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::recalculateViewMatrix() {
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position)
			* glm::rotate(glm::mat4(1.0), glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMatrix = glm::inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
