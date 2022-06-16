#include "sldpch.h"
#include "GuiElements.h"

#include "GuiTessellator.h"

#include "Salad/Renderer/TextureManager.h"
#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/Renderer2D.h"

namespace Salad {

	// -------- GuiElementWindowBackground --------- //

	GuiElementWindowBackground::GuiElementWindowBackground(GuiWindow* window) {

		Tessellator::start(TessellatorMode::TRIANGLES);

		if (m_FlatColor) {

			Tessellator::pushVertex(0, 0, 0, 0, 0);
			Tessellator::pushVertex(window->getWindowWidth(), 0, 0, 1, 0);
			Tessellator::pushVertex(0, window->getWindowHeight(), 0, 0, 1);
			Tessellator::pushVertex(window->getWindowWidth(), window->getWindowHeight(), 0, 1, 1);
			Tessellator::makeQuad();
		}
		else {
			float resolution = 1.0f / 1024.0f;
			Tessellator::setTextureResolution(resolution);
			Ref<WindowStyle> style = window->getWindowStyle();

			int ctlx = style->cornerTopLeftPosX;
			int ctly = style->cornerTopLeftPosY;
			int ctrx = style->cornerTopRightPosX;
			int ctry = style->cornerTopRightPosY;

			int cblx = style->cornerBottomLeftPosX;
			int cbly = style->cornerBottomLeftPosY;
			int cbrx = style->cornerBottomRightPosX;
			int cbry = style->cornerBottomRightPosY;

			int th = style->topHeight;
			int bh = style->bottomHeight;
			int lw = style->leftWidth;
			int rw = style->rightWidth;

			int topHorizontalLength = ctrx - (ctlx + lw);
			int bottomHorizontalLength = cbrx - (cblx + lw);
			int leftVerticalLength = cbly - (ctly + th);
			int rightVerticalLength = cbry - (ctry + th);

			int centreWidth = /*style->windowWidth*/ 256 - lw - rw;
			int centreHeight = /*style->windowHeight*/ 256 - bh - th;

			// Top left corner
			int posX = 0;
			int posY = 0;
			Tessellator::pushVertex(0, 0, 0, ctlx, ctly);
			Tessellator::pushVertex(lw, 0, 0, ctlx + lw, ctly);
			Tessellator::pushVertex(0, th, 0, ctlx, ctly + th);
			Tessellator::pushVertex(lw, th, 0, ctlx + lw, ctly + th);
			Tessellator::makeQuad();

			// Top right corner
			posX = window->getWindowWidth() - rw;
			posY = 0;
			Tessellator::pushVertex(posX, posY, 0, ctrx, ctry);
			Tessellator::pushVertex(posX + rw, posY, 0, ctrx + rw, ctry);
			Tessellator::pushVertex(posX, posY + th, 0, ctrx, ctry + th);
			Tessellator::pushVertex(posX + rw, posY + th, 0, ctrx + rw, ctry + th);
			Tessellator::makeQuad();

			// Bottom left corner
			posX = 0;
			posY = window->getWindowHeight() - bh;
			Tessellator::pushVertex(posX, posY, 0, cblx, cbly);
			Tessellator::pushVertex(posX + lw, posY, 0, cblx + lw, cbly);
			Tessellator::pushVertex(posX, posY + bh, 0, cblx, cbly + bh);
			Tessellator::pushVertex(posX + lw, posY + bh, 0, cblx + lw, cbly + bh);
			Tessellator::makeQuad();

			// Bottom right corner
			posX = window->getWindowWidth() - rw;
			posY = window->getWindowHeight() - bh;
			Tessellator::pushVertex(posX, posY, 0, cbrx, cbry);
			Tessellator::pushVertex(posX + rw, posY, 0, cbrx + rw, cbry);
			Tessellator::pushVertex(posX, posY + bh, 0, cbrx, cbry + bh);
			Tessellator::pushVertex(posX + rw, posY + bh, 0, cbrx + rw, cbry + bh);
			Tessellator::makeQuad();

			// Top Row
			posX = lw;
			posY = 0;
			while (posX < window->getWindowWidth() - rw) {
				int toDraw = std::min((int)((window->getWindowWidth() - rw) - posX), topHorizontalLength);
				Tessellator::pushVertex(posX, posY, 0, ctlx + lw, ctly);
				Tessellator::pushVertex(posX + toDraw, posY, 0, ctlx + lw + toDraw, ctly);
				Tessellator::pushVertex(posX, posY + th, 0, ctlx + lw, ctly + th);
				Tessellator::pushVertex(posX + toDraw, posY + th, 0, ctlx + lw + toDraw, ctly + th);
				Tessellator::makeQuad();

				posX += topHorizontalLength;
			}

			// Bottom row
			posX = lw;
			posY = window->getWindowHeight() - bh;
			while (posX < window->getWindowWidth() - rw) {
				int toDraw = std::min((int)((window->getWindowWidth() - rw) - posX), bottomHorizontalLength);
				Tessellator::pushVertex(posX, posY, 0, cblx + lw, cbly);
				Tessellator::pushVertex(posX + toDraw, posY, 0, cblx + lw + toDraw, cbly);
				Tessellator::pushVertex(posX, posY + bh, 0, cblx + lw, cbly + bh);
				Tessellator::pushVertex(posX + toDraw, posY + bh, 0, cblx + lw + toDraw, cbly + bh);
				Tessellator::makeQuad();

				posX += bottomHorizontalLength;
			}

			// Left Row
			posX = 0;
			posY = th;
			while (posY < window->getWindowHeight() - bh) {
				int toDraw = std::min((int)((window->getWindowHeight() - bh) - posY), leftVerticalLength);
				Tessellator::pushVertex(posX, posY, 0, ctlx, ctly + th);
				Tessellator::pushVertex(posX + lw, posY, 0, ctlx + lw, ctly + th);
				Tessellator::pushVertex(posX, posY + toDraw, 0, ctlx, ctly + th + toDraw);
				Tessellator::pushVertex(posX + lw, posY + toDraw, 0, ctlx + lw, ctly + th + toDraw);
				Tessellator::makeQuad();
				posY += leftVerticalLength;
			}

			// Right Row
			posX = window->getWindowWidth() - rw;
			posY = th;
			while (posY < window->getWindowHeight() - bh) {
				int toDraw = std::min((int)((window->getWindowHeight() - bh) - posY), rightVerticalLength);
				Tessellator::pushVertex(posX, posY, 0, ctrx, ctry + th);
				Tessellator::pushVertex(posX + rw, posY, 0, ctrx + rw, ctry + th);
				Tessellator::pushVertex(posX, posY + toDraw, 0, ctrx, ctry + th + toDraw);
				Tessellator::pushVertex(posX + rw, posY + toDraw, 0, ctrx + rw, ctry + th + toDraw);
				Tessellator::makeQuad();
				posY += rightVerticalLength;
			}

			// Centre

			posX = lw;
			posY = th;

			int testx = window->getWindowWidth() - rw;

			while (posX < testx) {
				int toDrawX = std::min((int)((testx)-posX), centreWidth);
				while (posY < window->getWindowHeight() - bh) {
					int toDrawY = std::min((int)((window->getWindowHeight() - bh) - posY), centreHeight);

					Tessellator::pushVertex(posX, posY, 0, ctlx + lw, ctry + th);
					Tessellator::pushVertex(posX + toDrawX, posY, 0, ctlx + lw + toDrawX, ctry + th);
					Tessellator::pushVertex(posX, posY + toDrawY, 0, ctlx + lw, ctry + th + toDrawY);
					Tessellator::pushVertex(posX + toDrawX, posY + toDrawY, 0, ctlx + lw + toDrawX, ctry + th + toDrawY);
					Tessellator::makeQuad();

					posY += centreHeight;
				}
				posX += centreWidth;
				posY = th;
			}
		}

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		m_RenderId = VertexArray::create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertexData[0], vertexData.size() * sizeof(float), SLD_STATIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
			});

		m_RenderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&indexData[0], indexData.size());
		m_RenderId->setIndexBuffer(indexBuffer);
		m_RenderId->unbind();
	}

	void GuiElementWindowBackground::init(GuiWindow* window) {

	}

	void GuiElementWindowBackground::draw(GuiWindow* window) {
		Renderer2D::drawVertexArray({ window->getWindowPosX(), window->getWindowPosY(), -0.99f }, { 1.0f, 1.0f }, m_RenderId);
	}

	// -------- GuiElementWindowBackground End --------- //


	// -------- GuiElementInteratable Start --------- //

	GuiElementInteractable::GuiElementInteractable(int elementId, int posX, int posY, int width, int height) :
		GuiElement(posX, posY, width, height),
		m_ElementId(elementId),
		m_PosX(posX),
		m_PosY(posY),
		m_Width(width),
		m_Height(height)	
	{}

	bool GuiElementInteractable::onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool hover = mousePosX > m_PosX && mousePosX < m_PosX + m_Width && mousePosY > m_PosY && mousePosY < m_PosY + m_Height;
		if (hover && m_State == GuiInteractableState::Hovering) {
			setState(window, GuiInteractableState::Pressed);
			window->onElementClicked(m_ElementId, button);
			onElementClicked(window, button);
		}

		return hover;
	}

	bool GuiElementInteractable::onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool hover = mousePosX > m_PosX && mousePosX < m_PosX + m_Width && mousePosY > m_PosY && mousePosY < m_PosY + m_Height;
		if (m_State == GuiInteractableState::Pressed) {
			setState(window, hover ? GuiInteractableState::Hovering : GuiInteractableState::Idle);
			window->onElementReleased(m_ElementId, button);
			onElementReleased(window, button);
		}

		return hover;
	}

	bool GuiElementInteractable::onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) {
		bool hover = mousePosX > m_PosX && mousePosX < m_PosX + m_Width && mousePosY > m_PosY && mousePosY < m_PosY + m_Height;
		if (hover && m_State == GuiInteractableState::Idle) {
			setState(window, GuiInteractableState::Hovering);
		}
		else if (!hover && m_State == GuiInteractableState::Hovering) {
			setState(window, GuiInteractableState::Idle);
		}
		return false;
	}

	void GuiElementInteractable::setState(GuiWindow* window, GuiInteractableState state) {
		if (state != m_State) {
			m_State = state;
			onStateChanged(window, state);
		}
	}

	// -------- GuiElementInteratable End --------- //


	// -------- GuiButton Start --------- //

	GuiButton::GuiButton(GuiWindow* window, int x, int y, int width, int height) :
		m_ButtonPosX(x),
		m_ButtonPosY(y),
		m_ButtonWidth(width),
		m_ButtonHeight(height) 	{
		Tessellator::start(TessellatorMode::TRIANGLES);

		float resolution = 1.0f / 1024.0f;
		Tessellator::setTextureResolution(resolution);
		Ref<WindowStyle> style = window->getWindowStyle();

		Tessellator::pushQuad(0, 0, 0.0f, width, height, 256.0f, 0.0f, 256, 32);

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		m_RenderId = VertexArray::create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertexData[0], vertexData.size() * sizeof(float), SLD_DYNAMIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
			});

		m_VertexBufferId = m_RenderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&indexData[0], indexData.size());
		m_RenderId->setIndexBuffer(indexBuffer);
		m_RenderId->unbind();
	}

	void GuiButton::init(GuiWindow* window) {

	}

	void GuiButton::draw(GuiWindow* window) {
		Renderer2D::drawVertexArray({ window->getWindowPosX() + m_ButtonPosX, window->getWindowPosY() + m_ButtonPosY }, { 1.0f, 1.0f }, m_RenderId);
	}

	bool GuiButton::onMouseButtonPressed(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool hover = mousePosX > m_ButtonPosX && mousePosX < m_ButtonPosX + m_ButtonWidth && mousePosY > m_ButtonPosY && mousePosY < m_ButtonPosY + m_ButtonHeight;
		if (hover && m_ButtonState == GuiInteractableState::Hovering) {
			setButtonState(window, GuiInteractableState::Pressed);
		}

		return hover;
	}

	bool GuiButton::onMouseButtonReleased(GuiWindow* window, int mousePosX, int mousePosY, int button) {
		bool hover = mousePosX > m_ButtonPosX && mousePosX < m_ButtonPosX + m_ButtonWidth && mousePosY > m_ButtonPosY && mousePosY < m_ButtonPosY + m_ButtonHeight;
		if (m_ButtonState == GuiInteractableState::Pressed) {
			setButtonState(window, hover ? GuiInteractableState::Hovering : GuiInteractableState::Idle);
			if (m_ButtonPressedCallback) m_ButtonPressedCallback();
		}

		return false;
	}

	bool GuiButton::onMouseMoveEvent(GuiWindow* window, int mousePosX, int mousePosY) {
		bool hover = mousePosX > m_ButtonPosX && mousePosX < m_ButtonPosX + m_ButtonWidth && mousePosY > m_ButtonPosY && mousePosY < m_ButtonPosY + m_ButtonHeight;
		if (hover && m_ButtonState == GuiInteractableState::Idle) {
			setButtonState(window, GuiInteractableState::Hovering);
		}
		else if (!hover && m_ButtonState == GuiInteractableState::Hovering) {
			setButtonState(window, GuiInteractableState::Idle);
		}

		return false;
	}

	void GuiButton::setButtonPressedCallback(void (*callback_func)()) {
		m_ButtonPressedCallback = callback_func;
	}

	void GuiButton::setButtonState(GuiWindow* window, GuiInteractableState state) {
		m_ButtonState = state;

		int buttonV = window->getWindowStyle()->buttonLeftPosY;
		switch (state) {
			case GuiInteractableState::Idle:
				buttonV += window->getWindowStyle()->buttonHeight * window->getWindowStyle()->buttonIdle;
				break;

			case GuiInteractableState::Hovering:
				buttonV += window->getWindowStyle()->buttonHeight * window->getWindowStyle()->buttonHovering;
				break;

			case GuiInteractableState::Pressed:
				buttonV += window->getWindowStyle()->buttonHeight * window->getWindowStyle()->buttonPressed;
				break;

			case GuiInteractableState::Disabled:
				buttonV += window->getWindowStyle()->buttonHeight * window->getWindowStyle()->buttonDisabled;
				break;

		}

		Tessellator::start(TessellatorMode::TRIANGLES);

		float resolution = 1.0f / 1024.0f;
		Tessellator::setTextureResolution(resolution);
		Ref<WindowStyle> style = window->getWindowStyle();

		Tessellator::pushQuad(0, 0, 0.0f, m_ButtonWidth, m_ButtonHeight, 256.0f, buttonV, 256, 32);

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		m_RenderId->updateVertexBuffer(0, 0, vertexData.size() * sizeof(float), &vertexData[0]);
		Tessellator::end();
	}

	// -------- GuiButton End --------- //


	// -------- GuiCheckbox Start --------- //

	GuiCheckbox::GuiCheckbox(GuiWindow* window, int elementId, int x, int y) :
		GuiElementInteractable(elementId, x, y, 24, 24) 	{
		Tessellator::start(TessellatorMode::TRIANGLES);

		float resolution = 1.0f / 1024.0f;
		Tessellator::setTextureResolution(resolution);
		Ref<WindowStyle> style = window->getWindowStyle();

		Tessellator::pushQuad(0, 0, 0.0f, m_Width, m_Height, 256.0f, 128.0f, m_Width, m_Height);

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		m_RenderId = VertexArray::create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertexData[0], vertexData.size() * sizeof(float), SLD_DYNAMIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
			});

		m_RenderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&indexData[0], indexData.size());
		m_RenderId->setIndexBuffer(indexBuffer);
		m_RenderId->unbind();
	}

	void GuiCheckbox::init(GuiWindow* window) {

	}

	void GuiCheckbox::draw(GuiWindow* window) {
		Renderer2D::drawVertexArray({ window->getWindowPosX() + m_PosX, window->getWindowPosY() + m_PosY }, { 1.0f, 1.0f }, m_RenderId);
	}

	void GuiCheckbox::onElementReleased(GuiWindow* window, int button) {
		m_Checked = !m_Checked;
		onStateChanged(window, m_State);
	}

	void GuiCheckbox::onStateChanged(GuiWindow* window, GuiInteractableState state) {
		int shiftX = 0;
		int shiftY = m_Checked;
		switch (state) {
			case GuiInteractableState::Idle:
				shiftX = window->getWindowStyle()->checkboxIdle;
				break;

			case GuiInteractableState::Hovering:
				shiftX = window->getWindowStyle()->checkboxHover;
				break;

			case GuiInteractableState::Pressed:
				shiftX = window->getWindowStyle()->checkboxPressed;
				break;

			case GuiInteractableState::Disabled:
				shiftX = window->getWindowStyle()->checkboxDisabled;
				break;

		}

		int checkboxU = window->getWindowStyle()->checkboxX + (window->getWindowStyle()->checkboxWidth * shiftX);
		int checkboxV = window->getWindowStyle()->checkboxY + (window->getWindowStyle()->checkboxHeight * shiftY);

		Tessellator::start(TessellatorMode::TRIANGLES);

		float resolution = 1.0f / 1024.0f;
		Tessellator::setTextureResolution(resolution);
		Ref<WindowStyle> style = window->getWindowStyle();

		Tessellator::pushQuad(0, 0, 0.0f, m_Width, m_Height, checkboxU, checkboxV, m_Width, m_Height);

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		m_RenderId->updateVertexBuffer(0, 0, vertexData.size() * sizeof(float), &vertexData[0]);
		Tessellator::end();
	}

	// -------- GuiCheckbox End --------- //


	// -------- GuiTextbox Start --------- //

	GuiTextbox::GuiTextbox(GuiWindow* window, int elementId, int x, int y, int width, int height, uint32_t maxLength) :
		GuiElementInteractable(elementId, x, y, width, height),
		m_ContentLength(maxLength), m_ContentCurrentLength(0), m_ContentIndexPointer(0)
	{
		m_Content = (char*)malloc(sizeof(char) * m_ContentLength);

		tesselateElement(window);

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		m_RenderId = VertexArray::create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertexData[0], vertexData.size() * sizeof(float), SLD_DYNAMIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
		});

		m_RenderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&indexData[0], indexData.size());
		m_RenderId->setIndexBuffer(indexBuffer);
		m_RenderId->unbind();

		m_Font = FontManager::loadFont("assets/fonts/arial.ttf", 14);
	}

	GuiTextbox::~GuiTextbox() {
		free((char*)m_Content);
	}

	void GuiTextbox::tesselateElement(GuiWindow* window) {
		Tessellator::start(TessellatorMode::TRIANGLES);
		Tessellator::setTextureResolution(1.0f / 1024.0f);

		GuiTessellator::tessellateTiledElement(0, 0, m_Width, m_Height, m_Focused ? window->getWindowStyle()->textboxActive : window->getWindowStyle()->textbox);
	}

	void GuiTextbox::init(GuiWindow* window) {

	}

	void GuiTextbox::draw(GuiWindow* window) {
		RenderCommand::enableScissorTest(true);
		GuiBounds absBounds = getAbsoluteBounds();
		RenderCommand::scissor(-1, -1, 300, 300);

		if(m_ContentCurrentLength > 0) {
			m_Font->getFontTexture()->bind();
			Salad::Renderer2D::drawVertexArray({ window->getWindowPosX() + m_PosX + 4, window->getWindowPosY() + m_PosY + 16 }, { 1.0f, 1.0f }, m_TextRenderId);
			TextureManager::get().getTexture2D("assets/textures/ui.png")->bind();
		}
		Renderer2D::drawVertexArray({ window->getWindowPosX() + m_PosX, window->getWindowPosY() + m_PosY }, { 1.0f, 1.0f }, m_RenderId);
		RenderCommand::enableScissorTest(false);
	}

	bool GuiTextbox::onKeyPressedEvent(GuiWindow* window, int keycode, int repeatCount) {
		switch(keycode) {
			// Return
			case 259:
				if (m_ContentIndexPointer > 0) { 
					m_ContentIndexPointer--;
					m_ContentCurrentLength--;
					onContentChanged();
				}
				break;
		}
		return false;
	}

	bool GuiTextbox::onKeyTypedEvent(GuiWindow* window, int keycode, char c) {
		if (m_ContentIndexPointer == m_ContentCurrentLength || m_TypeMode == 1)
			m_ContentCurrentLength++;
		
		m_Content[m_ContentIndexPointer++] = c;

		onContentChanged();
		return true;
	}

	void GuiTextbox::onElementClicked(GuiWindow* window, int button) {

	}

	void GuiTextbox::setFocus(GuiWindow* window, bool focus) {
		m_Focused = focus;
		tesselateElement(window);
			
		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		m_RenderId->updateVertexBuffer(0, 0, vertexData.size() * sizeof(float), &vertexData[0]);

		m_Focused = focus;
	}

	void GuiTextbox::onContentChanged() {
		if (m_ContentCurrentLength > 0) {
			m_TextRenderId = m_Font->generateText(getContent());
		}
	}

	std::string GuiTextbox::getContent() {
		std::string text(m_Content, m_ContentCurrentLength);
		return text;
	}

	// -------- GuiTextbox End --------- //

	// -------- GuiLabel Start --------- //

	GuiLabel::GuiLabel(int x, int y, std::string& text) :
		m_PosX(x),
		m_PosY(y) 	{
		m_Font = FontManager::loadFont("assets/fonts/arial.ttf", 20);
		m_Text = m_Font->generateText(text);
	}

	void GuiLabel::init(GuiWindow* window) {

	}

	void GuiLabel::draw(GuiWindow* window) {
		m_Font->getFontTexture()->bind();
		Salad::Renderer2D::drawVertexArray({ window->getWindowPosX() + m_PosX, window->getWindowPosY() + m_PosY }, { 1.0f, 1.0f }, m_Text);
	}

	// -------- GuiLabel End --------- //

}