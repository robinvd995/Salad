#pragma once

#include "Salad/Core/Input.h"

namespace Salad {

	class WindowsInput : public Input {

	protected:
		virtual void pollInputsImpl() override;

		virtual bool isKeyPressedImpl(int keycode) override;
		virtual bool isMouseButtonPressedImpl(int button) override;
		virtual float getMouseXImpl() override;
		virtual float getMouseYImpl() override;
		virtual float getMouseDeltaXImpl() override;
		virtual float getMouseDeltaYImpl() override;
		virtual void setMousePosImpl(float mouseX, float mouseY) override;
		virtual void setMousePosRelImpl(float mouseX, float mouseY) override;
		virtual std::pair<float, float> getMousePositionImpl() override;

	private:
		std::pair<float, float> m_MousePos = { 0.0f, 0.0f };
		std::pair<float, float> m_LastMousePos = { 0.0f, 0.0f };
		std::pair<float, float> m_MouseDelta = { 0.0f, 0.0f };

		std::pair<float, float> m_MousePosToSet = { 0.0f, 0.0f };
		bool m_NeedToSetMousePos = false;
	};
}