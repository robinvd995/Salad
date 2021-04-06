#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Salad/Events/ApplicationEvent.h"
#include "Salad/Core/Timestep.h"

#include "Salad/ImGui/ImGuiLayer.h"

namespace Salad {

	// TODO: Maybe change this?
	struct ApplicationVariables {
		bool Flag_ImGuiBlockEvents = false;
	};

	class Application
	{
	public:
		Application(const std::string& appname = "Salad Application");
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void close();

		inline static ApplicationVariables& variables() { return s_Instance->m_AppVariables; }
		inline static Application& get() { return *s_Instance; }
		inline Window& getWindow() { return *m_Window; }

	private:
		bool onWindowClose(WindowCloseEvent& e);
		bool onWindowResize(WindowResizeEvent& e);
	private:
		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = false;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
		//Ref<Camera> m_Camera;

		ApplicationVariables m_AppVariables;

	private:
		static Application* s_Instance;
	};

	//Define in client
	Application* createApplication();
}
