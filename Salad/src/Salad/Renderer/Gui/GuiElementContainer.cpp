#include "sldpch.h"
#include "GuiElementContainer.h"

namespace Salad {

	GuiElementContainer::GuiElementContainer() {}
	GuiElementContainer::~GuiElementContainer() {}

	bool GuiElementContainer::onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool handled = false;
		for(int i = 0; i < m_Elements.size(); i++) {
			handled = m_Elements[i]->onMouseButtonPressed(window, mousePosX, mousePosY, button);
			if (handled) break;
		}
		return handled;
	}

	bool GuiElementContainer::onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool handled = false;
		for (int i = 0; i < m_Elements.size(); i++) {
			handled = m_Elements[i]->onMouseButtonReleased(window, mousePosX, mousePosY, button);
			if (handled) break;
		}
		return handled;
	}

	bool GuiElementContainer::onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) {
		bool handled = false;
		for (int i = 0; i < m_Elements.size(); i++) {
			handled = m_Elements[i]->onMouseMoveEvent(window, mousePosX, mousePosY);
			if (handled) break;
		}
		return handled;
	}

	bool GuiElementContainer::onKeyPressedEvent(GuiWindow* window, int keycode, int repeatCount) {
		return false;
	}

	bool GuiElementContainer::onKeyReleasedEvent(GuiWindow* window, int keycode) {
		return false;
	}

	bool GuiElementContainer::onKeyTypedEvent(GuiWindow* window, int keycode, char c) {
		return false;
	}

	void GuiElementContainer::draw(GuiWindow* window) {}

	GuiElement* GuiElementContainer::doesClickFocusElement(GuiWindow* window, uint32_t mousePosX, uint32_t mousePosY) {
		for (int i = 0; i < m_Elements.size(); i++) {
			GuiElement* focused = m_Elements[i]->doesClickFocusElement(window, mousePosX, mousePosY);
			if(focused) return focused;
		}
		return nullptr;
	}
}
