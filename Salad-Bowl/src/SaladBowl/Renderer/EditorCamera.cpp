#include "EditorCamera.h"

#include "Salad/Core/Core.h"

#include "Salad/Core/Input.h"
#include "Salad/Core/KeyCodes.h"

#include <glm/glm.hpp>
#include <glm/glm/ext/matrix_transform.hpp>

#include "SaladBowl/EditorSettings.hpp"

namespace Salad {

	const bool EditorCamera::s_LockMouseForMode[] = { false, true, true, true };
	const bool EditorCamera::s_MouseScrollEnabled[] = { true, true, false, false };

	EditorCamera::EditorCamera() :
		Camera(Salad::PerspectiveCameraProperties()) 
	{
		recalcEditorViewMatrix();
	}

	void EditorCamera::init() {
		EditorSettings::pushGroup("Editor Camera");

		EditorSettings::pushSubGroup("Freeflight Mode");
		EditorSettings::pushFloat("Sensitivity", &m_ControlSettings.ff_Sensitivity);
		EditorSettings::pushBool("Invert Horizontal", &m_ControlSettings.ff_InvertHorizontal);
		EditorSettings::pushBool("Invert Vertical", &m_ControlSettings.ff_InvertVertical);
		EditorSettings::popSubGroup();

		EditorSettings::pushSubGroup("Orbit Mode");
		EditorSettings::pushFloat("Sensitivity", &m_ControlSettings.or_Sensitivity);
		EditorSettings::pushBool("Invert Horizontal", &m_ControlSettings.or_InvertHorizontal);
		EditorSettings::pushBool("Invert Vertical", &m_ControlSettings.or_InvertVertical);
		EditorSettings::popSubGroup();

		EditorSettings::pushSubGroup("Scroll Mode");
		EditorSettings::pushFloat("Sensitivity", &m_ControlSettings.sc_Sensitivity);
		EditorSettings::pushBool("Invert Horizontal", &m_ControlSettings.sc_InvertHorizontal);
		EditorSettings::pushBool("Invert Vertical", &m_ControlSettings.sc_InvertVertical);
		EditorSettings::popSubGroup();

		EditorSettings::pushFloat("Max Camera Angle", &m_ControlSettings.maxCameraAngle);

		EditorSettings::popGroup();
	}

	void EditorCamera::updateCamera(Timestep ts) {

		m_PrevMode = m_Mode;

		bool rightMouseDown = Input::isMouseButtonPressed(1);     // TODO: make it into a setting
		bool middleMouseDown = Input::isMouseButtonPressed(2);    // TODO: make it into a setting
		bool shiftDown = Input::isKeyPressed(SLD_KEY_LEFT_SHIFT); // TODO: make it into a setting

		if (rightMouseDown && shiftDown) m_Mode = EditorCameraMode::Freeflight;
		else if (rightMouseDown) m_Mode = EditorCameraMode::Orbit;
		else if (middleMouseDown) m_Mode = EditorCameraMode::Scroll;
		else m_Mode = EditorCameraMode::None;

		int curModeIndex = static_cast<int>(m_Mode);
		bool curShouldLockMouse = s_LockMouseForMode[curModeIndex];

		if(m_Mode != m_PrevMode) {
			int prevModeIndex = static_cast<int>(m_PrevMode);
			bool prevShouldLockMouse = s_LockMouseForMode[prevModeIndex];

			if(curShouldLockMouse != prevShouldLockMouse) {
				Input::showCursor(!curShouldLockMouse);

				if(curShouldLockMouse) {
					m_MousePosXLock = Input::getMouseX();
					m_MousePosYLock = Input::getMouseY();
				}
			}
		}

		switch(m_Mode) {
			case EditorCameraMode::None: break;
			case EditorCameraMode::Freeflight: freeflight(ts); break;
			case EditorCameraMode::Orbit: orbit(ts); break;
			case EditorCameraMode::Scroll: scroll(ts); break;
		}

		if (curShouldLockMouse) Input::setMousePos(m_MousePosXLock, m_MousePosYLock);
	}

	void EditorCamera::orbit(Timestep ts) {
		float speed = m_ControlSettings.or_Sensitivity * ts * 100;

		float mouseDeltaX = Input::getMouseDeltaX();
		float mouseDeltaY = -Input::getMouseDeltaY();

		m_Rotation.y -= mouseDeltaX * speed * (m_ControlSettings.ff_InvertHorizontal ? -1.0f : 1.0f);
		m_Rotation.x += mouseDeltaY * speed * (m_ControlSettings.ff_InvertVertical ? -1.0f : 1.0f);

		if (m_Rotation.x > m_ControlSettings.maxCameraAngle) {
			m_Rotation.x = m_ControlSettings.maxCameraAngle;
		}
		if (m_Rotation.x < -m_ControlSettings.maxCameraAngle) {
			m_Rotation.x = -m_ControlSettings.maxCameraAngle;
		}

		float distanceToFocus = glm::distance(m_Position, m_FocusPoint);
		float posX = 0.0f, posY = 0.0f, posZ = 0.0f;

		posZ += cos(glm::radians(-m_Rotation.y)) * cos(glm::radians(-m_Rotation.x)) * distanceToFocus;
		posX += sin(glm::radians(-m_Rotation.y)) * cos(glm::radians(m_Rotation.x)) * distanceToFocus;
		posY += sin(glm::radians(m_Rotation.x)) * distanceToFocus;

		m_Position.x = m_FocusPoint.x + posX;
		m_Position.y = m_FocusPoint.y + posY;
		m_Position.z = m_FocusPoint.z + posZ;

		recalcEditorViewMatrix();
	}

	void EditorCamera::scroll(Timestep ts) {
		float speed = m_ControlSettings.sc_Sensitivity * ts;
		
		float mouseDeltaX = Input::getMouseDeltaX() * (m_ControlSettings.sc_InvertHorizontal ? -1.0f : 1.0f);;
		float mouseDeltaY = -Input::getMouseDeltaY() * (m_ControlSettings.sc_InvertVertical ? -1.0f : 1.0f);;

		float deltaX = 0.0f, deltaY = 0.0f, deltaZ = 0.0f;

		deltaX += cos(glm::radians(m_Rotation.y)) * speed * mouseDeltaX;
		deltaZ += sin(glm::radians(m_Rotation.y)) * speed * mouseDeltaX;

		deltaY += cos(glm::radians(-m_Rotation.x)) * speed * mouseDeltaY;
		deltaZ += sin(glm::radians(-m_Rotation.x)) * speed * mouseDeltaY;
		deltaX += cos(glm::radians(-m_Rotation.x)) * speed * mouseDeltaY;

		m_Position.x += deltaX;
		m_Position.y += deltaY;
		m_Position.z += deltaZ;

		m_FocusPoint.x += deltaX;
		m_FocusPoint.y += deltaY;
		m_FocusPoint.z += deltaZ;

		recalcEditorViewMatrix();
	}

	void EditorCamera::freeflight(Timestep ts) {
	
		/*float delta = (float)ts;
		float speed = 5.0f * delta;

		if (Input::isKeyPressed(SLD_KEY_W)) {
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

		float mouseDeltaX = Input::getMouseDeltaX();
		float mouseDeltaY = -Input::getMouseDeltaY();

		m_Yaw += mouseDeltaX * m_ControlSettings.ff_Sensitivity * (m_ControlSettings.ff_InvertHorizontal ? -1.0f : 1.0f);
		m_Pitch += mouseDeltaY * m_ControlSettings.ff_Sensitivity * (m_ControlSettings.ff_InvertVertical ? -1.0f : 1.0f);

		if (m_Pitch > 80.0f) {
			m_Pitch = 80.0f;
		}
		if (m_Pitch < -80.0f) {
			m_Pitch = -80.0f;
		}

		/*glm::vec3 direction{ 0.0f };
		direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		direction.y = sin(glm::radians(m_Pitch));
		direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_CameraFront = glm::normalize(direction);

		m_ViewMatrix = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
		recalcEditorViewMatrix();*/
	}

	void EditorCamera::recalcEditorViewMatrix() {

		// TODO: Might not be calculated correctly
		glm::mat4 matrix = glm::mat4(1.0f);
		matrix = glm::rotate(matrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		matrix = glm::rotate(matrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		matrix = glm::translate(matrix, -m_Position);

		m_ViewMatrix = matrix; // glm::inverse(matrix);
	}

	void EditorCamera::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(SLD_BIND_EVENT_FN(EditorCamera::onMouseScrolledEvent));
	}

	bool EditorCamera::onMouseScrolledEvent(MouseScrolledEvent& e) {

		bool canScroll = s_MouseScrollEnabled[static_cast<int>(m_Mode)];

		if(canScroll) {

			float distanceToFocus = glm::distance(m_Position, m_FocusPoint);
			float strengthBasedOnDistance = pow(distanceToFocus, 1.2) * 0.05f;

			float zoomIntensity = 0.5f * e.getYOffset() * strengthBasedOnDistance;

			float x = 0.0f, y = 0.0f, z = 0.0f;

			z += glm::cos(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x));
			x += glm::sin(glm::radians(m_Rotation.y)) * glm::cos(glm::radians(m_Rotation.x));
			y += sin(glm::radians(m_Rotation.x));

			float dx = x * zoomIntensity;
			float dy = y * -zoomIntensity;
			float dz = z * -zoomIntensity;

			m_Position.x += dx;
			m_Position.y += dy;
			m_Position.z += dz;

			if(Input::isKeyPressed(SLD_KEY_LEFT_SHIFT)) {
				m_FocusPoint.x += dx;
				m_FocusPoint.y += dy;
				m_FocusPoint.z += dz;
			}

			recalcEditorViewMatrix();
		}

		return canScroll;
	}

}