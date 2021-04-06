#pragma once

#define _USE_MATH_DEFINES

#include "Salad/Core/Timestep.h"

#include "Salad/Renderer/Camera.h"

#include <cmath>
#include <Salad/Events/MouseEvent.h>

namespace Salad {

	struct EditorCameraControlSettings {
	
		float ff_Sensitivity = 0.05f;
		bool ff_InvertHorizontal = false, ff_InvertVertical = false;

		float or_Sensitivity = 0.05f;
		bool or_InvertHorizontal = false, or_InvertVertical = false;

		float sc_Sensitivity = 2.0f;
		bool sc_InvertHorizontal = false, sc_InvertVertical = false;

		float maxCameraAngle = 85.0f;
	
		EditorCameraControlSettings() = default;
	};

	enum class EditorCameraMode {
		None        = 0,
		Orbit       = 1,
		Scroll      = 2,
		Freeflight  = 3
	};

	class EditorCamera : public Camera {

	public:

		EditorCamera();

		void updateCamera(Timestep ts);
		void onEvent(Event& e);

		glm::mat4& getViewMatrix() { return m_ViewMatrix; }
		
		EditorCameraControlSettings& getCameraControlSettings() { return m_ControlSettings; }

	private:
		bool onMouseScrolledEvent(MouseScrolledEvent& e);

	private:
		void orbit(Timestep ts);
		void scroll(Timestep ts);
		void freeflight(Timestep ts);

		void recalcEditorViewMatrix();

	private:
		glm::mat4 m_ViewMatrix{ 1.0f };

		glm::vec3 m_CameraPos = { -10.0f, 10.0f, -10.0f };
		glm::vec3 m_CameraFront = { 0.0f, 0.0f, -1.0f };
		glm::vec3 m_CameraUp = { 0.0f, 1.0f, 0.0f };

		glm::vec3 m_Position{ 10.0f, 5.0f, 10.0f };
		glm::vec3 m_Rotation{ 20.0f, -45.0f, 0.0f };

		//float m_Yaw = 45.0f, m_Pitch = -30.0f;

		EditorCameraControlSettings m_ControlSettings;

		float m_MousePosXLock = 0.0f, m_MousePosYLock = 0.0f;

		EditorCameraMode m_Mode = EditorCameraMode::None;
		EditorCameraMode m_PrevMode = EditorCameraMode::None;

		glm::vec3 m_FocusPoint{ 0.0f };

		static const bool s_LockMouseForMode[];
		static const bool s_MouseScrollEnabled[];
	};

}