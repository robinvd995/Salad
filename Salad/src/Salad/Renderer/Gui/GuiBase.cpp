#include "sldpch.h"
#include "GuiBase.h"

namespace Salad {

	GuiElement* GuiElement::doesClickFocusElement(GuiWindow* window, uint32_t mousePosX, uint32_t mousePosY) {
		if (canFocus() && isOnElement(mousePosX, mousePosY)) return this;
		return nullptr;
	}
}