#pragma once

#include "Salad/Events/Event.h"
#include "Salad/Renderer/Texture.h"
#include "Salad/Renderer/VertexArray.h"
#include "Salad/Renderer/Gui/FontManager.h"
#include "Salad/Events/MouseEvent.h"
#include "Salad/Events/ApplicationEvent.h"

namespace Salad {

	class GuiWindow;

	class GuiElement {
	
	public:

		virtual void init(GuiWindow* window) = 0;
		virtual void draw(GuiWindow* window) = 0;

		virtual bool onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) { return false; }
		virtual bool onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) { return false; }
		virtual bool onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) { return false; }

	private:
	
	};

	struct TiledElementStyle {
		int posX = 0, posY = 0;
		int width = 0, height = 0;
		int leftWidth = 0, rightWidth = 0, topHeight = 0, bottomHeight = 0;

		TiledElementStyle() {}
	};

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
		bool onWindowResizeEvent(WindowResizeEvent& e);

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

		bool m_IsVisible = true;
		bool m_IsLocationLocked = false;

		int m_RelMousePosX = 0, m_RelMousePosY = 0;

		bool m_Dragged = false;
		int m_DragOffsetX = 0, m_DragOffsetY = 0;

		Ref<WindowStyle> m_WindowStyle;

		uint32_t m_ScreenWidth = 0, m_ScreenHeight = 0;
	};

	class GuiElementWindowBackground : public GuiElement {

	public:
		GuiElementWindowBackground(GuiWindow* window);
		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;

	private:
		Ref<VertexArray> m_RenderId;
		bool m_FlatColor = false;
	};

	enum class GuiInteractableState {
		Idle = 0,
		Hovering = 1,
		Pressed = 2,
		Disabled = 3
	};

	class GuiElementInteractable : public GuiElement {

	public:

		GuiElementInteractable(int elementId, int posX, int posY, int width, int height);

		virtual void init(GuiWindow* window) = 0;
		virtual void draw(GuiWindow* window) = 0;

		virtual bool onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) override;

		virtual void onMouseEnter() {}
		virtual void onMouseLeave() {}
		virtual void onElementClicked(GuiWindow* window, int button) {}
		virtual void onElementReleased(GuiWindow* window, int button) {}

	protected:
		void setState(GuiWindow* window, GuiInteractableState state);
		virtual void onStateChanged(GuiWindow* window, GuiInteractableState state) {}

	protected:
		int m_ElementId;
		int m_PosX, m_PosY;
		int m_Width, m_Height;

		GuiInteractableState m_State = GuiInteractableState::Idle;

	};

	class GuiButton : public GuiElement {
	
	public:
		GuiButton(GuiWindow* window, int x, int y, int width, int height);
		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;

		virtual bool onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) override;
		virtual bool onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) override;

	private:
		void setButtonState(GuiWindow* window, GuiInteractableState state);

	private:
		int m_ButtonPosX, m_ButtonPosY;
		int m_ButtonWidth, m_ButtonHeight;
		
		GuiInteractableState m_ButtonState = GuiInteractableState::Idle;

		Ref<VertexArray> m_RenderId;
		uint32_t m_VertexBufferId;
	};

	class GuiCheckbox : public GuiElementInteractable {
	
	public:
		GuiCheckbox(GuiWindow* window, int elementId, int x, int y);
	
		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;

		virtual void onElementReleased(GuiWindow* window, int button) override;

		bool isChecked() { return m_Checked; }

	protected:
		virtual void onStateChanged(GuiWindow* window, GuiInteractableState state) override;

	private:
		Ref<VertexArray> m_RenderId;
		bool m_Checked = false;

	};

	class GuiLabel : public GuiElement { 
	
	public:
		GuiLabel(int x, int y, std::string& text);

		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;

	private:
		int m_PosX, m_PosY;
		Ref<Font> m_Font;
		Ref<VertexArray> m_Text;
	};

	class GuiTextbox : public GuiElementInteractable { 
	
	public:
		GuiTextbox(GuiWindow* window, int elementId, int x, int y, int width, int height);
		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;
	
	private:
		Ref<VertexArray> m_RenderId;
	};
}
