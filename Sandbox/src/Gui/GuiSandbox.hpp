#include "Salad/Core/Layer.h"

#include <iostream>

#include <Salad.h>

namespace Salad {

	class GuiSandbox : public Layer {

	public:
		GuiSandbox() : Layer("GuiSandbox") {}
		~GuiSandbox() = default;

		static void onButtonPressed() {
			std::cout << "Button pressed!" << std::endl;
		}

		virtual void onAttach() override {

			// Initializing the renderer
			GuiRenderer::initializeRenderer();

			// Gui window setup
			m_GuiWindow = Salad::createRef<Salad::GuiWindow>(10, 10, 0.0f, 0.0f, 500, 400);
			Salad::Ref<Salad::WindowStyle> windowStyle = Salad::GuiWindow::loadWindowStyle("assets/misc/window_styles/default.lua");
			m_GuiWindow->setWindowStyle(windowStyle);
			m_GuiWindow->initWindow();

			Ref<GuiButton> button = createRef<GuiButton>(m_GuiWindow.get(), 20, 108, 256, 32);
			button->setButtonPressedCallback(&onButtonPressed);
			m_GuiWindow->addElement(button);

			m_GuiWindow->addElement(createRef<GuiElementWindowBackground>(m_GuiWindow.get()));
			m_GuiWindow->addElement(createRef<GuiCheckbox>(m_GuiWindow.get(), 0, 20, 80));

			//Ref<GuiTextbox> textbos = createRef<GuiTextbox>(m_GuiWindow.get());
			m_GuiWindow->addElement(createRef<GuiTextbox>(m_GuiWindow.get(), 1, 52, 80, 224, 24));
			m_GuiWindow->addElement(createRef<GuiLabel>(88, 28, std::string("Hello World!")));

			// Camera transform setup
			m_CameraTransform.setPosition(0.0f, 0.0f, 0.0f);
			m_CameraTransform.setOrientation(glm::quat());
			m_CameraTransform.setScale(1.0f, 1.0f, 1.0f);
			m_CameraTransform.calculateTransformationMatrix();

			// Camera projection setup
			m_Camera.setProjectionType(Salad::Camera::CameraType::Orthographic);
			m_Camera.setOrthographicProjection(OrthographicCameraProperties{ 1920.0f, 1080.0f });
			m_Camera.recalculateViewMatrix();

			std::cout << "GuiLayer attached!" << std::endl;
		}

		virtual void onDetach() override {

		}

		virtual void onUpdate(Timestep ts) override {
			
			Salad::Renderer2D::beginScene(m_Camera, m_CameraTransform.getMatrix());
			
			//Salad::Renderer2D::drawQuad(glm::vec3(960.0, 540.0, 0.0f), glm::vec2(1920.0f, 1080.0f), glm::vec4(0.5f, 0.3f, 1.0f, 1.0f));
			m_GuiWindow->drawWindow();

			Salad::Renderer2D::endScene();
		}

		virtual void onImGuiRender() override {

		}

		bool onWindowResizeEvent(WindowResizeEvent& e) {
			m_Camera.setProjectionType(Salad::Camera::CameraType::Orthographic);
			m_Camera.setOrthographicProjection(OrthographicCameraProperties{ (float)e.getWidth(), (float) e.getHeight() });
			m_Camera.recalculateViewMatrix();

			Renderer::onWindowResized((uint32_t)e.getWidth(), (uint32_t)e.getHeight());

			std::cout << "resizing, width: " << e.getWidth() << ", height: " << e.getHeight() << std::endl;

			return false;
		}

		virtual void onEvent(Event& event) override {
			Salad::EventDispatcher dispatcher(event);
			dispatcher.dispatch<WindowResizeEvent>(SLD_BIND_EVENT_FN(GuiSandbox::onWindowResizeEvent));
			m_GuiWindow->onEvent(event);
		}

	private:
		
		Salad::Camera m_Camera;
		Salad::Transform m_CameraTransform;

		Salad::Ref<Salad::GuiWindow> m_GuiWindow;

		Salad::Ref<Salad::Font> m_Font;
		Salad::Ref<Salad::VertexArray> m_Text;

	};

}

