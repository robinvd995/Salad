#pragma once

#include "Salad/Core/Timestep.h"
#include "Salad/Events/ApplicationEvent.h"
#include "Salad/Events/MouseEvent.h"
#include "Salad/Renderer/OrthographicCamera.h"

namespace Salad {

	class OrthographicCameraController {

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void onUpdate(Timestep ts);
		void onEvent(Event& e);
		OrthographicCamera getCamera() const { return m_Camera; }
		
	private:
		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onWindowResizedEvent(WindowResizeEvent& e);
	private:
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;
		bool m_Rotation;

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1.0f, m_CameraRotationSpeed = 180.0f;
	};

}