#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Salad/Events/ApplicationEvent.h"
#include "Salad/Core/Timestep.h"

#include "Salad/ImGui/ImGuiLayer.h"

#include "Salad/Renderer/OrthographicCamera.h"

namespace Salad {

	class SALAD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
		void onEvent(Event& e);

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

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

	private:
		static Application* s_Instance;
	};

	//Define in client
	Application* createApplication();
}
