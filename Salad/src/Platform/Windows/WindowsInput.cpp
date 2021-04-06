#include "sldpch.h"
#include "Salad/Core/Input.h"

#include <math.h>

#include <GLFW/glfw3.h>

#include "Salad/Core/Application.h"

namespace Salad {

	static struct MouseData {
		std::pair<float, float> MousePos        { 0.0f, 0.0f };
		std::pair<float, float> LastMousePos    { 0.0f, 0.0f };
		std::pair<float, float> MouseDelta      { 0.0f, 0.0f };
		std::pair<float, float> MousePosToSet   { 0.0f, 0.0f };
		bool NeedToSetMousePos                  { false };
		bool CenterMouse                        { false };

	} s_WindowsInputData;

	void Input::poll() {
		s_WindowsInputData.MousePos = getMousePosition();

		s_WindowsInputData.MouseDelta.first = s_WindowsInputData.MousePos.first - s_WindowsInputData.LastMousePos.first;
		s_WindowsInputData.MouseDelta.second = s_WindowsInputData.MousePos.second - s_WindowsInputData.LastMousePos.second;

		if (s_WindowsInputData.NeedToSetMousePos) {
			GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
			glfwSetCursorPos(window, s_WindowsInputData.MousePosToSet.first, s_WindowsInputData.MousePosToSet.second);
			s_WindowsInputData.NeedToSetMousePos = false;
			s_WindowsInputData.LastMousePos.first = s_WindowsInputData.MousePosToSet.first;
			s_WindowsInputData.LastMousePos.second = s_WindowsInputData.MousePosToSet.second;
		}
		else {
			s_WindowsInputData.LastMousePos.first = s_WindowsInputData.MousePos.first;
			s_WindowsInputData.LastMousePos.second = s_WindowsInputData.MousePos.second;
		}

		if(s_WindowsInputData.CenterMouse)
			setMousePosRel(0.5f, 0.5f);
	}

	bool Input::isKeyPressed(int keycode) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::isMouseButtonPressed(int button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	float Input::getMouseX() {
		auto [x, y] = getMousePosition();
		return x;
	}

	float Input::getMouseY() {
		auto[x, y] = getMousePosition();
		return y;
	}

	float Input::getMouseDeltaX() {
		return s_WindowsInputData.MouseDelta.first;
	}

	float Input::getMouseDeltaY() {
		return s_WindowsInputData.MouseDelta.second;
	}

	void Input::setMousePos(float mouseX, float mouseY) {
		s_WindowsInputData.MousePosToSet.first = mouseX;
		s_WindowsInputData.MousePosToSet.second = mouseY;
		s_WindowsInputData.NeedToSetMousePos = true;
	}

	void Input::setMousePosRel(float mouseX, float mouseY) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		int width = 0, height = 0;
		glfwGetWindowSize(window, &width, &height);
		s_WindowsInputData.MousePosToSet.first = std::floor((float) width * mouseX);
		s_WindowsInputData.MousePosToSet.second = std::floor((float) height * mouseY);
		s_WindowsInputData.NeedToSetMousePos = true;
	}

	std::pair<float, float> Input::getMousePosition() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);
		return { (float)xpos, (float)ypos };
	}

	void Input::lockMouseToCenter(bool shouldLock) {
		s_WindowsInputData.CenterMouse = shouldLock;
	}

	void Input::showCursor(bool shouldShow) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		glfwSetInputMode(window, GLFW_CURSOR, shouldShow ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
	}

	/*void Input::setMousePos(float x, float y) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
		glfwSetCursorPos(window, x, y);
	}*/
}