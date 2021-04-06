#include "sldpch.h"
#include "Camera.h"

#include <glm/gtx/quaternion.hpp>

namespace Salad {

	Camera::Camera() {
		setPerspectiveProjection(PerspectiveCameraProperties());
	}

	Camera::Camera(CameraType projectionType, const glm::mat4& projection) :
		m_CameraProjectionType(projectionType),
		m_Projection(projection)
	{}

	Camera::Camera(PerspectiveCameraProperties props) {
		setPerspectiveProjection(props);
	}

	Camera::Camera(OrthographicCameraProperties props) {
		setOrthographicProjection(props);
	}

	Camera::~Camera() {

	}

	void Camera::recalculateViewMatrix() {
		switch(m_CameraProjectionType) {
			case CameraType::Perspective: m_Projection = glm::perspective(
				glm::radians(m_PerspectiveProperties.Fov), m_PerspectiveProperties.AspectRatio, 
				m_PerspectiveProperties.NearClip, m_PerspectiveProperties.FarClip); 
				break;
			case CameraType::Orthographic: m_Projection = glm::ortho(
				m_OrthographicProperties.Left, m_OrthographicProperties.Right,
				m_OrthographicProperties.Bottom, m_OrthographicProperties.Top, 
				m_OrthographicProperties.ZNear, m_OrthographicProperties.ZFar); 
				break;
		}
	}
}
