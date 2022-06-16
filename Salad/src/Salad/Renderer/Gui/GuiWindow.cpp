#include "sldpch.h"
#include "GuiWindow.h"

#include "GuiTessellator.h"

#include "Salad/Renderer/TextureManager.h"
#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/Renderer2D.h"

#include "Salad/Lua/LuaScript.h"

#include <algorithm>
#include <Salad\Renderer\Gui\FontManager.h>

namespace Salad {

	// -------- GuiWindow --------- //

	GuiWindow::GuiWindow(int offsetX, int offsetY, float allignX, float allignY, uint32_t width, uint32_t height) :
		m_OffsetX(offsetX),
		m_OffsetY(offsetY),
		m_AllignX(allignX),
		m_AllignY(allignY),
		m_WindowWidth(width),
		m_WindowHeight(height) 	{
	}

	void GuiWindow::initWindow() {
		Ref<Texture2D> texture = TextureManager::get().loadTexture2D("assets/textures/ui.png");
		/*m_Elements.push_back(createRef<GuiCheckbox>(this, 0, 20, 80));
		m_Elements.push_back(createRef<GuiTextbox>(this, 1, 52, 80, 224, 24));
		m_Elements.push_back(createRef<GuiButton>(this, 20, 108, 256, 32));
		m_Elements.push_back(createRef<GuiLabel>(88, 28, std::string("Hello World!")));
		m_Elements.push_back(createRef<GuiElementWindowBackground>(this));*/
	}

	void GuiWindow::destroyWindow() {

	}

	void GuiWindow::drawWindow() {
		for (int i = 0; i < m_Elements.size(); i++) {
			TextureManager::get().getTexture2D("assets/textures/ui.png")->bind();
			m_Elements[i]->draw(this);
		}
	}

	void GuiWindow::onEvent(Event& e) {
		Salad::EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseButtonPressedEvent>(SLD_BIND_EVENT_FN(GuiWindow::onMouseButtonPressed));
		dispatcher.dispatch<MouseButtonReleasedEvent>(SLD_BIND_EVENT_FN(GuiWindow::onMouseButtonReleased));
		dispatcher.dispatch<MouseMoveEvent>(SLD_BIND_EVENT_FN(GuiWindow::onMouseMoveEvent));

		dispatcher.dispatch<KeyPressedEvent>(SLD_BIND_EVENT_FN(GuiWindow::onKeyPressedEvent));
		dispatcher.dispatch<KeyReleasedEvent>(SLD_BIND_EVENT_FN(GuiWindow::onKeyReleasedEvent));
		dispatcher.dispatch<KeyTypedEvent>(SLD_BIND_EVENT_FN(GuiWindow::onKeyTypedEvent));

		dispatcher.dispatch<WindowResizeEvent>(SLD_BIND_EVENT_FN(GuiWindow::onWindowResizeEvent));
	}

	bool GuiWindow::onMouseButtonPressed(MouseButtonPressedEvent& e) {

		bool handled = false;
		bool focused = false;
		for (int i = 0; i < m_Elements.size(); i++) {

			if (m_Elements[i]->canFocus() && m_Elements[i]->isOnElement(m_RelMousePosX, m_RelMousePosY)) {
				if(m_HasFocusedElement) m_FocusedElement->setFocus(this, false);
				m_FocusedElement = m_Elements[i];
				m_FocusedElement->setFocus(this, true);
				m_HasFocusedElement = true;
				focused = true;
			}

			handled = m_Elements[i]->onMouseButtonPressed(this, m_RelMousePosX, m_RelMousePosY, e.getMouseButton());
			if (handled) break;
		}

		if (!focused && m_HasFocusedElement) {
			m_FocusedElement->setFocus(this, false);
			m_HasFocusedElement = false;
		}

		if (!handled && !isLocationLocked() && m_RelMousePosX > 0 && m_RelMousePosX < m_WindowWidth && m_RelMousePosY > 0 && m_RelMousePosY < m_WindowStyle->topHeight) {
			m_Dragged = true;
			m_DragOffsetX = m_RelMousePosX;
			m_DragOffsetY = m_RelMousePosY;
		}

		return handled;
	}

	bool GuiWindow::onMouseButtonReleased(MouseButtonReleasedEvent& e) {

		bool handled = false;
		for (int i = 0; i < m_Elements.size(); i++) {
			handled = m_Elements[i]->onMouseButtonReleased(this, m_RelMousePosX, m_RelMousePosY, e.getMouseButton());
			if (handled) break;
		}

		if (!handled)
			m_Dragged = false;

		return handled;
	}

	bool GuiWindow::onMouseMoveEvent(MouseMoveEvent& e) {

		m_RelMousePosX = e.getX() - m_WindowPosX;
		m_RelMousePosY = e.getY() - m_WindowPosY;

		bool handled = false;
		for (int i = 0; i < m_Elements.size(); i++) {
			handled = m_Elements[i]->onMouseMoveEvent(this, m_RelMousePosX, m_RelMousePosY);
			if (handled) break;
		}

		if (!handled && m_Dragged) {
			m_WindowPosX = e.getX() - m_DragOffsetX;
			m_WindowPosY = e.getY() - m_DragOffsetY;

			if (m_WindowPosX < 0) m_WindowPosX = 0;
			if (m_WindowPosY < 0) m_WindowPosY = 0;
		}

		return handled;
	}

	bool GuiWindow::onKeyPressedEvent(KeyPressedEvent& e) {
		if(m_HasFocusedElement && m_FocusedElement) {
			return m_FocusedElement->onKeyPressedEvent(this, e.getKeyCode(), e.getRepeatCount());
		}
		else {
			// Handle own key events
			return false;
		}
	}

	bool GuiWindow::onKeyReleasedEvent(KeyReleasedEvent& e) {
		if (m_HasFocusedElement && m_FocusedElement) {
			return m_FocusedElement->onKeyReleasedEvent(this, e.getKeyCode());
		}
		else {
			// Handle own key events
			return false;
		}
	}

	bool GuiWindow::onKeyTypedEvent(KeyTypedEvent& e) {
		if (m_HasFocusedElement && m_FocusedElement) {
			return m_FocusedElement->onKeyTypedEvent(this, e.getKeyCode(), (char) e.getKeyCode());
		}
		else {
			// Handle own key events
			return false;
		}
	}

	bool GuiWindow::onWindowResizeEvent(WindowResizeEvent& e) {

		m_WindowPosX = m_OffsetX + (m_AllignX * e.getWidth());
		m_WindowPosY = m_OffsetY + (m_AllignY * e.getHeight());

		m_ScreenWidth = e.getWidth();
		m_ScreenHeight = e.getHeight();
		return false;
	}

	void GuiWindow::addElement(Ref<GuiElement> element) {
		m_Elements.push_back(element);
	}

	Ref<WindowStyle> GuiWindow::loadWindowStyle(const char* filepath) {
		LuaScript script(filepath);
		Ref<WindowStyle> windowStyle = createRef<WindowStyle>();

		script.scopeToTable("window");

		windowStyle->cornerTopLeftPosX = script.getInt("CornerTopLeftX");
		windowStyle->cornerTopLeftPosY = script.getInt("CornerTopLeftY");
		windowStyle->cornerTopRightPosX = script.getInt("CornerTopRightX");
		windowStyle->cornerTopRightPosY = script.getInt("CornerTopRightY");
		windowStyle->cornerBottomLeftPosX = script.getInt("CornerBottomLeftX");
		windowStyle->cornerBottomLeftPosY = script.getInt("CornerBottomLeftY");
		windowStyle->cornerBottomRightPosX = script.getInt("CornerBottomRightX");
		windowStyle->cornerBottomRightPosY = script.getInt("CornerBottomRightY");

		windowStyle->topHeight = script.getInt("TopHeight");
		windowStyle->bottomHeight = script.getInt("BottomHeight");
		windowStyle->leftWidth = script.getInt("LeftWidth");
		windowStyle->rightWidth = script.getInt("RightWidth");

		script.scopeToTable("button");

		windowStyle->buttonLeftPosX = script.getInt("LeftX");
		windowStyle->buttonLeftPosY = script.getInt("LeftY");
		windowStyle->buttonRightPosX = script.getInt("RightX");
		windowStyle->buttonRightPosY = script.getInt("RightY");

		windowStyle->buttonLeftWidth = script.getInt("LeftWidth");
		windowStyle->buttonRightWidth = script.getInt("RightWidth");
		windowStyle->buttonHeight = script.getInt("Height");

		windowStyle->buttonIdle = script.getInt("Idle");
		windowStyle->buttonDisabled = script.getInt("Disabled");
		windowStyle->buttonHovering = script.getInt("Hover");
		windowStyle->buttonPressed = script.getInt("Pressed");

		script.scopeToTable("checkbox");

		windowStyle->checkboxX = script.getInt("X");
		windowStyle->checkboxY = script.getInt("Y");
		windowStyle->checkboxWidth = script.getInt("Width");
		windowStyle->checkboxHeight = script.getInt("Height");

		windowStyle->checkboxIdle = script.getInt("Idle");
		windowStyle->checkboxHover = script.getInt("Hover");
		windowStyle->checkboxPressed = script.getInt("Pressed");
		windowStyle->checkboxDisabled = script.getInt("Disabled");

		script.scopeToTable("textbox");

		windowStyle->textbox.posX = script.getInt("X");
		windowStyle->textbox.posY = script.getInt("Y");
		windowStyle->textbox.width = script.getInt("Width");
		windowStyle->textbox.height = script.getInt("Height");

		windowStyle->textbox.leftWidth = script.getInt("LeftWidth");
		windowStyle->textbox.rightWidth = script.getInt("RightWidth");
		windowStyle->textbox.topHeight = script.getInt("TopHeight");
		windowStyle->textbox.bottomHeight = script.getInt("BottomHeight");

		script.scopeToTable("textboxActive");

		windowStyle->textboxActive.posX = script.getInt("X");
		windowStyle->textboxActive.posY = script.getInt("Y");
		windowStyle->textboxActive.width = script.getInt("Width");
		windowStyle->textboxActive.height = script.getInt("Height");

		windowStyle->textboxActive.leftWidth = script.getInt("LeftWidth");
		windowStyle->textboxActive.rightWidth = script.getInt("RightWidth");
		windowStyle->textboxActive.topHeight = script.getInt("TopHeight");
		windowStyle->textboxActive.bottomHeight = script.getInt("BottomHeight");

		return windowStyle;
	}

	void GuiWindow::onElementClicked(int elementId, int mouseButton) {

	}

	void GuiWindow::onElementReleased(int elementId, int mouseButton) {

	}

	// -------- GuiWindow End --------- //


}
