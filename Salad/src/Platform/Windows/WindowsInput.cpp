#include "sldpch.h"
#include "WindowsInput.h"

#include "Salad/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Salad {

	Input* Input::s_Instance = new WindowsInput();

	void WindowsInput::pollInputsImpl() {
		m_MousePos = getMousePositionImpl();

		m_MouseDelta.first = m_MousePos.first - m_LastMousePos.first;
		m_MouseDelta.second = m_MousePos.second - m_LastMousePos.second;

		if (m_NeedToSetMousePos) {
			GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
			glfwSetCursorPos(window, m_MousePosToSet.first, m_MousePosToSet.second);
			m_NeedToSetMousePos = false;
			m_LastMousePos.first = m_MousePosToSet.first;
			m_LastMousePos.second = m_MousePosToSet.second;
		}
		else {
			m_LastMousePos.first = m_MousePos.first;
			m_LastMousePos.second = m_MousePos.second;
		}
	}

	bool WindowsInput::isKeyPressedImpl(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::isMouseButtonPressedImpl(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float WindowsInput::getMouseXImpl() {
		auto [x, y] = getMousePositionImpl();
		return x;
	}

	float WindowsInput::getMouseYImpl() {
		auto[x, y] = getMousePositionImpl();
		return y;
	}

	float WindowsInput::getMouseDeltaXImpl() {
		return m_MouseDelta.first;
	}

	float WindowsInput::getMouseDeltaYImpl() {
		return m_MouseDelta.second;
	}

	void WindowsInput::setMousePosImpl(float mouseX, float mouseY) {
		m_MousePosToSet.first = mouseX;
		m_MousePosToSet.second = mouseY;
		m_NeedToSetMousePos = true;
	}

	void WindowsInput::setMousePosRelImpl(float mouseX, float mouseY) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);
		m_MousePosToSet.first = (float) width * mouseX;
		m_MousePosToSet.second = (float) height * mouseY;
		m_NeedToSetMousePos = true;
	}

	std::pair<float, float> WindowsInput::getMousePositionImpl() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

}