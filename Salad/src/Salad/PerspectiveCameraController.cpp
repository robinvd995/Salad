#include "sldpch.h"
#include "PerspectiveCameraController.h"

#include "Salad/Core/Input.h"
#include "Salad/Core/KeyCodes.h"

namespace Salad {

	PerspectiveCameraController::PerspectiveCameraController(PerspectiveCameraProps props) :
		m_Camera(props)
	{}

	void PerspectiveCameraController::onUpdate(Timestep ts) {
		float delta = (float)ts;
		float speed = 5.0f * delta;

		if(Input::isKeyPressed(SLD_KEY_W)) {
			m_CameraPos += speed * m_CameraFront;
		}
		if (Input::isKeyPressed(SLD_KEY_S)) {
			m_CameraPos -= speed * m_CameraFront;
		}
		if (Input::isKeyPressed(SLD_KEY_A)) {
			m_CameraPos -= glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
		}
		if (Input::isKeyPressed(SLD_KEY_D)) {
			m_CameraPos += glm::normalize(glm::cross(m_CameraFront, m_CameraUp)) * speed;
		}

		if ((Input::isKeyPressed(SLD_KEY_ESCAPE)) && !m_Pressed) {
			m_Pressed = true;
			m_Active = !m_Active;
			SLD_CORE_INFO("Active={0}", m_Active);
		}

		if (!(Input::isKeyPressed(SLD_KEY_ESCAPE)) && m_Pressed) {
			m_Pressed = false;
		}

		if (m_Active) {
			const float sensitivity = 0.05f;

			float mouseDeltaX = Input::getMouseDeltaX();
			float mouseDeltaY = - Input::getMouseDeltaY();

			m_Yaw += mouseDeltaX * sensitivity;
			m_Pitch += mouseDeltaY * sensitivity;

			if (m_Pitch > 80.0f) {
				m_Pitch = 80.0f;
			}
			if (m_Pitch < -80.0f) {
				m_Pitch = -80.0f;
			}

			glm::vec3 direction;
			direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			direction.y = sin(glm::radians(m_Pitch));
			direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_CameraFront = glm::normalize(direction);

			Input::setMousePosRel(0.5f, 0.5f);
		}

		m_Camera.setViewMatrix(glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp));
	}

	void PerspectiveCameraController::onEvent(Event& e) {

	}
}