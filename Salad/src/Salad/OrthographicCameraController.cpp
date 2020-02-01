#include "sldpch.h"
#include "OrthographicCameraController.h"

#include "Salad/Core/Input.h"
#include "Salad/Core/KeyCodes.h"

namespace Salad {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation) :
		m_AspectRatio(aspectRatio),
		m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		m_Rotation(rotation)
	{}

	void OrthographicCameraController::onUpdate(Timestep ts) {

		float delta = (float)ts;
		float speed = m_CameraTranslationSpeed * delta * m_ZoomLevel;

		if (Input::isKeyPressed(SLD_KEY_A)) {
			m_CameraPosition.x -= speed;
		}

		if (Input::isKeyPressed(SLD_KEY_D)) {
			m_CameraPosition.x += speed;
		}

		if (Input::isKeyPressed(SLD_KEY_W)) {
			m_CameraPosition.y += speed;
		}

		if (Input::isKeyPressed(SLD_KEY_S)) {
			m_CameraPosition.y -= speed;
		}

		if (Input::isKeyPressed(SLD_KEY_Q)) {
			m_CameraRotation -= m_CameraRotationSpeed * delta;
		}

		if (Input::isKeyPressed(SLD_KEY_E)) {
			m_CameraRotation += m_CameraRotationSpeed * delta;
		}

		m_Camera.setPosition(m_CameraPosition);
		if (m_Rotation) {
			m_Camera.setRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(SLD_BIND_EVENT_FN(OrthographicCameraController::onMouseScrolledEvent));
		dispatcher.dispatch<WindowResizeEvent>(SLD_BIND_EVENT_FN(OrthographicCameraController::onWindowResizedEvent));
	}

	bool OrthographicCameraController::onMouseScrolledEvent(MouseScrolledEvent& e) {
		m_ZoomLevel -= e.getYOffset() * 0.1f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_ZoomLevel = std::min(m_ZoomLevel, 4.0f);
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::onWindowResizedEvent(WindowResizeEvent& e) {
		m_AspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_Camera.setProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}