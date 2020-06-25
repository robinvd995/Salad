#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Events/Event.h"

namespace Salad {

	class SALAD_API Input {

	public:
		inline static void poll() { s_Instance->pollInputsImpl(); }
		inline static bool isKeyPressed(int keycode) { return s_Instance->isKeyPressedImpl(keycode); }
		inline static bool isMouseButtonPressed(int button) { return s_Instance->isMouseButtonPressedImpl(button); }
		inline static float getMouseX() { return s_Instance->getMouseXImpl(); }
		inline static float getMouseY() { return s_Instance->getMouseYImpl(); }
		inline static float getMouseDeltaX() { return s_Instance->getMouseDeltaXImpl(); }
		inline static float getMouseDeltaY() { return s_Instance->getMouseDeltaYImpl(); }
		inline static void setMousePos(float mouseX, float mouseY) { s_Instance->setMousePosImpl(mouseX, mouseY); }
		inline static void setMousePosRel(float mouseX, float mouseY) { s_Instance->setMousePosRelImpl(mouseX, mouseY); }
		inline static std::pair<float,float> getMousePosition() { return s_Instance->getMousePositionImpl(); }

	protected:
		virtual void pollInputsImpl() = 0;

		virtual bool isKeyPressedImpl(int keycode) = 0;
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual float getMouseXImpl() = 0;
		virtual float getMouseYImpl() = 0;
		virtual float getMouseDeltaXImpl() = 0;
		virtual float getMouseDeltaYImpl() = 0;
		virtual void setMousePosImpl(float mouseX, float mouseY) = 0;
		virtual void setMousePosRelImpl(float mouseX, float mouseY) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;

	private:
		static Input* s_Instance;
	};

}