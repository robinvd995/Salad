#pragma once

#include "GuiBase.h"

#include "Salad/Events/Event.h"
#include "Salad/Events/MouseEvent.h"
#include "Salad/Events/KeyEvent.h"
#include "Salad/Events/ApplicationEvent.h"

namespace Salad {

	class GuiElementContainer : public GuiElement {
	
	public:
		GuiElementContainer();
		~GuiElementContainer();

		virtual bool onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) override;

		virtual bool onKeyPressedEvent(GuiWindow* window, int keycode, int repeatCount) override;
		virtual bool onKeyReleasedEvent(GuiWindow* window, int keycode) override;
		virtual bool onKeyTypedEvent(GuiWindow* window, int keycode, char c) override;

		virtual void draw(GuiWindow* window) override;

		//TODO: Rename function
		virtual GuiElement* doesClickFocusElement(GuiWindow* window, uint32_t mousePosX, uint32_t mousePosY) override;

	private:
		std::vector<Ref<GuiElement>> m_Elements;

	};

}
