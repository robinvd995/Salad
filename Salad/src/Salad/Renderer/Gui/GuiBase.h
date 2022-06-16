#pragma once

namespace Salad {

	class GuiWindow;

	struct GuiBounds {
		uint32_t x, y, width, height;

		//GuiBounds() :
		//	x(0), y(0), width(32), height(32)
		//{}

		GuiBounds(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height) :
			x(p_x), y(p_y), width(p_width), height(p_height)
		{}

		GuiBounds add(GuiBounds& other) {
			return { x + other.x, y + other.y, width, height };
		}
	};

	struct TiledElementStyle {
		int posX = 0, posY = 0;
		int width = 0, height = 0;
		int leftWidth = 0, rightWidth = 0, topHeight = 0, bottomHeight = 0;

		TiledElementStyle() {}
	};

	class GuiElement {

	public:

		GuiElement() : 
			m_Bounds({0, 0, 32, 32}) 
		{}

		GuiElement(uint32_t x, uint32_t y, uint32_t width, uint32_t height) :
			m_Bounds({x, y, width, height})
		{}

		~GuiElement() {}

		virtual void init(GuiWindow* window) = 0;
		virtual void draw(GuiWindow* window) = 0;

		virtual bool onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) { return false; }
		virtual bool onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) { return false; }
		virtual bool onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) { return false; }

		virtual bool onKeyPressedEvent(GuiWindow* window, int keycode, int repeatCount) { return false; }
		virtual bool onKeyReleasedEvent(GuiWindow* window, int keycode) { return false; }
		virtual bool onKeyTypedEvent(GuiWindow* window, int keycode, char c) { return false; }

		virtual bool canFocus() { return false; }
		virtual void setFocus(GuiWindow* window, bool focus) {}
		virtual bool isOnElement(uint32_t posX, uint32_t posY) { return false; }

		virtual GuiBounds& getGuiBounds() { return m_Bounds; }
		// Make an absolute bound and change it only when the parent element or itself has changed, send the event down to child elements
		virtual GuiBounds getAbsoluteBounds() { return m_Parent ? m_Bounds.add(m_Parent->getAbsoluteBounds()) : m_Bounds; }

		virtual GuiElement* doesClickFocusElement(GuiWindow* window, uint32_t mousePosX, uint32_t mousePosY);

		// If gui element uses a different texture compared to the GuiWindow
		virtual bool hasTextureOverride() { return m_HasTextureOverride; }
		// The texture the gui uses
		virtual std::string& getTextureOverride() { return m_TextureOverride; }
		// The order in which the gui gets rendered, lower = first
		virtual uint32_t getGuiRenderLayer() { return m_GuiRenderLayer; }

	protected:
		GuiElement* m_Parent = nullptr;
		GuiBounds m_Bounds;

		uint32_t m_GuiRenderLayer = 0;
		uint32_t m_HasTextureOverride = false;
		std::string m_TextureOverride = "";

		// TODO: getAbsoluteBounds()
		//GuiBounds m_AbsoluteBounds;
	};
}