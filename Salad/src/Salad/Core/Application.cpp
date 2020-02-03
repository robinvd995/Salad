#include "sldpch.h"
#include "Application.h"

#include "Salad/Core/Log.h"

#include "Input.h"

#include "Salad/Renderer/Renderer.h"
#include "Salad/Renderer/RenderCommand.h"

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Salad {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
	{
		SLD_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = Scope<Window>(Window::create());
		m_Window->setEventCallback(BIND_EVENT_FN(onEvent));
		m_Window->setVSync(true);

		Renderer::init();

		m_ImGuiLayer = new ImGuiLayer();
		pushOverlay(m_ImGuiLayer);

		m_Running = true;
	}

	Application::~Application() {

	}

	void Application::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowClose));
		dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));
		//SLD_CORE_TRACE("{0}", e);

		for (LStackIt it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->onEvent(e);
			if (e.isHandled())
				break;
		}
	}

	void Application::pushLayer(Layer* layer) {
		m_LayerStack.pushLayer(layer);
		layer->onAttach();
	}

	void Application::pushOverlay(Layer* overlay) {
		m_LayerStack.pushOverlay(overlay);
		overlay->onAttach();
	}

	void Application::run() {

		while (m_Running) {

			float time = (float) glfwGetTime(); // Platform::getTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			for (Layer* layer : m_LayerStack) {
				layer->onUpdate(timestep);
			}

			m_ImGuiLayer->begin();
			for (Layer* layer : m_LayerStack) {
				layer->onImGuiRender();
			}
			m_ImGuiLayer->end();

			m_Window->onUpdate();
		}
	}

	bool Application::onWindowClose(WindowCloseEvent& e) {
		m_Running = false;
		return true;
	}

	bool Application::onWindowResize(WindowResizeEvent& e) {
		if (e.getWidth() == 0 || e.getHeight() == 0) {
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		Renderer::onWindowResized(e.getWidth(), e.getHeight());

		return false;
	}
}
