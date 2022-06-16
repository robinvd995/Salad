#pragma once

#include "GuiBase.h"
#include "GuiElementContainer.h"

#include "Salad/Events/Event.h"
#include "Salad/Renderer/Texture.h"
#include "Salad/Renderer/VertexArray.h"
#include "Salad/Renderer/Gui/FontManager.h"
#include "Salad/Events/MouseEvent.h"
#include "Salad/Events/KeyEvent.h"
#include "Salad/Events/ApplicationEvent.h"

#include <vector>

namespace Salad {

	struct WindowStyle {
		int cornerTopLeftPosX = 0, cornerTopLeftPosY = 0;
		int cornerTopRightPosX = 0, cornerTopRightPosY = 0;
		int cornerBottomLeftPosX = 0, cornerBottomLeftPosY = 0;
		int cornerBottomRightPosX = 0, cornerBottomRightPosY = 0;
		int topHeight = 0, bottomHeight = 0, leftWidth = 0, rightWidth = 0;

		int buttonLeftPosX = 0, buttonLeftPosY = 0;
		int buttonRightPosX = 0, buttonRightPosY = 0;
		int buttonLeftWidth = 0, buttonRightWidth = 0, buttonHeight = 0;
		int buttonIdle = 0, buttonDisabled = 0, buttonHovering = 0, buttonPressed = 0;

		int checkboxX = 0, checkboxY = 0;
		int checkboxWidth = 0, checkboxHeight = 0;
		int checkboxIdle = 0, checkboxHover = 0, checkboxPressed = 0, checkboxDisabled = 0;

		TiledElementStyle textbox;
		TiledElementStyle textboxActive;

		WindowStyle() {}
	};

	class GuiWindow {

	public:

		GuiWindow(int offsetX, int offsetY, float allignX, float allignY, uint32_t width, uint32_t height);

		void initWindow();
		void destroyWindow();

		// TODO void addElement(Ref<GuiElement> element);
		void drawWindow();

		inline void showWindow(bool show) { m_IsVisible = show; }

		inline uint32_t getWindowPosX() const { return m_WindowPosX; }
		inline uint32_t getWindowPosY() const { return m_WindowPosY; }
		inline uint32_t getWindowWidth() const { return m_WindowWidth; }
		inline uint32_t getWindowHeight() const { return m_WindowHeight; }

		void onEvent(Event& e);
		bool onMouseButtonPressed(MouseButtonPressedEvent& e);
		bool onMouseButtonReleased(MouseButtonReleasedEvent& e);
		bool onMouseMoveEvent(MouseMoveEvent& e);

		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onKeyReleasedEvent(KeyReleasedEvent& e);
		bool onKeyTypedEvent(KeyTypedEvent& e);

		bool onWindowResizeEvent(WindowResizeEvent& e);

		void addElement(Ref<GuiElement> element);

		void setWindowStyle(Ref<WindowStyle> windowStyle) { m_WindowStyle = windowStyle; }
		Ref<WindowStyle> getWindowStyle() { return m_WindowStyle; }

		void setLockLocation(bool lockLocation) { m_IsLocationLocked = lockLocation; }
		bool isLocationLocked() const { return m_IsLocationLocked; }

		// TODO Could move this to AssetIO later on if needed
		static Ref<WindowStyle> loadWindowStyle(const char* filepath);

		void onElementClicked(int elementId, int mouseButton);
		void onElementReleased(int elementId, int mouseButton);

	private:
		int m_OffsetX, m_OffsetY;
		float m_AllignX, m_AllignY;
		int  m_WindowWidth, m_WindowHeight, m_WindowPosX = 0, m_WindowPosY = 0;

		Ref<Texture2D> m_Texture;
		//Ref<GuiElement> m_WindowBackgroundElement;
		//Ref<GuiElement> m_GuiButton;
		std::vector<Ref<GuiElement>> m_Elements;
		Ref<GuiElement> m_FocusedElement;
		bool m_HasFocusedElement = false;

		bool m_IsVisible = true;
		bool m_IsLocationLocked = false;

		int m_RelMousePosX = 0, m_RelMousePosY = 0;

		bool m_Dragged = false;
		int m_DragOffsetX = 0, m_DragOffsetY = 0;

		Ref<WindowStyle> m_WindowStyle;

		uint32_t m_ScreenWidth = 0, m_ScreenHeight = 0;
	};
}