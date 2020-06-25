#pragma once

#include "Salad/Renderer/Camera.h"
#include "Salad/Core/Timestep.h"
#include "Salad/Events/Event.h"

#include <glm/glm.hpp>

namespace Salad {

	class PerspectiveCameraController {
	
	public:
		PerspectiveCameraController(PerspectiveCameraProps props);
	
		virtual void onUpdate(Timestep ts);
		void onEvent(Event& e);
		PerspectiveCamera getCamera() const { return m_Camera; }
	private:
		PerspectiveCamera m_Camera;

		glm::vec3 m_CameraPos = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		float m_Yaw = 0.0f, m_Pitch = 0.0f;
		bool m_Active = false;
		bool m_Pressed = false;
	};

}