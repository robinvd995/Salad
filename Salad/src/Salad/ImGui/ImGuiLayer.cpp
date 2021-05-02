#include "sldpch.h"
#include "ImGuiLayer.h"

#include "Salad/Core/Application.h"
#include "imgui.h"
#include "examples/imgui_impl_glfw.h"
#include "examples/imgui_impl_opengl3.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <ImGuizmo.h>

namespace Salad {

	ImGuiLayer::ImGuiLayer() :
		Layer("ImGuiLayer"),
        m_ImGuiIniFile("default")
	{}

    ImGuiLayer::ImGuiLayer(const char* imGuiIniFile) :
        Layer("ImGuiLayer"),
        m_ImGuiIniFile(imGuiIniFile),
        m_HasCustomIniLocation(true)
    {
    }

	ImGuiLayer::~ImGuiLayer() {

	}

    void ImGuiLayer::begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::end() {
        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::get();
        io.DisplaySize = ImVec2((float) app.getWindow().getWidth(), (float) app.getWindow().getHeight());

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

	void ImGuiLayer::onAttach() {
        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        if (m_HasCustomIniLocation) io.IniFilename = m_ImGuiIniFile;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            style.WindowRounding = 0.0f;
            style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Setup Platform/Renderer bindings
        GLFWwindow* window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
	}

    void ImGuiLayer::onImGuiRender() {
        //static bool show = false;
        //ImGui::ShowDemoWindow(&show);
    }

    void ImGuiLayer::onEvent(Event& e) {
        if(Application::variables().Flag_ImGuiBlockEvents) {
            ImGuiIO& io = ImGui::GetIO();
            e.handled |= e.isInCategory(EventCategoryMouse) & io.WantCaptureMouse;
            e.handled |= e.isInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
        }
    }

}