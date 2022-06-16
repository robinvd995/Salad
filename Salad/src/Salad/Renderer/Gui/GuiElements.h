#pragma once

#include "Gui.h"

namespace Salad {

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

		virtual bool isOnElement(uint32_t posX, uint32_t posY) override { return posX > m_PosX && posX < m_PosX + m_Width && posY > m_PosY&& posY < m_PosY + m_Height; }

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

		void setButtonPressedCallback(void (*callback_func)());

	private:
		void setButtonState(GuiWindow* window, GuiInteractableState state);

	private:
		int m_ButtonPosX, m_ButtonPosY;
		int m_ButtonWidth, m_ButtonHeight;

		GuiInteractableState m_ButtonState = GuiInteractableState::Idle;

		Ref<VertexArray> m_RenderId;
		uint32_t m_VertexBufferId;

		void (*m_ButtonPressedCallback)() = nullptr;
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
		GuiTextbox(GuiWindow* window, int elementId, int x, int y, int width, int height, uint32_t maxLength = 32);
		~GuiTextbox();
		virtual void init(GuiWindow* window) override;
		virtual void draw(GuiWindow* window) override;

		virtual void onElementClicked(GuiWindow* window, int button) override;

		virtual bool onKeyPressedEvent(GuiWindow* window, int keycode, int repeatCount) override;
		//virtual bool onKeyReleasedEvent(GuiWindow* window, int keycode) override;
		virtual bool onKeyTypedEvent(GuiWindow* window, int keycode, char c) override;

		std::string getContent();

	private:
		void tesselateElement(GuiWindow* window);
		void onContentChanged();

		virtual bool canFocus() { return true; }
		virtual void setFocus(GuiWindow* window, bool focus) override;

	private:
		// Typemode, 0 = override, 1 = add
		uint32_t m_TypeMode = 0;
		const uint32_t m_ContentLength;
		uint32_t m_ContentCurrentLength;
		uint32_t m_ContentIndexPointer;
		char* m_Content = nullptr;
		bool m_Focused = false;

		Ref<VertexArray> m_RenderId;
		Ref<VertexArray> m_TextRenderId;
		Ref<Font> m_Font;
	};

}
