#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Events/Event.h"

namespace Salad {

	class Input {

	public:
		static void poll();
		static bool isKeyPressed(int keycode);
		static bool isMouseButtonPressed(int button);
		static float getMouseX();
		static float getMouseY();
		static float getMouseDeltaX();
		static float getMouseDeltaY();
		static void setMousePos(float mouseX, float mouseY);
		static void setMousePosRel(float mouseX, float mouseY);
		static std::pair<float, float> getMousePosition();
		static void lockMouseToCenter(bool shouldlock = true);
		static void showCursor(bool shouldShow);
	};

}