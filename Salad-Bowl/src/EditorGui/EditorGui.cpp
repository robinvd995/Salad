#include "EditorGui.h"

#include "ImGui/imgui.h"
#include "ImGuizmo.h"
#include "EditorStyle.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include "Salad/Math/Math.h"
#include "Salad/Core/Application.h"
#include "Salad/Scene/Components.h"
#include "Salad/Renderer/Renderer.h"

#include "Salad/Scene/Components.h"

#include "Panels/FileExplorerPanel.h"
#include "Panels/MaterialExplorerPanel.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ScenePropertiesPanel.h"
#include "Panels/LogPanel.h"
#include "Panels/ConsolePanel.h"

namespace Salad::EditorGui {

	EditorUI::EditorUI() {
		m_Panels.push_back(new SceneHierarchyPanel());
		m_Panels.push_back(new MaterialExplorerPanel());
		m_Panels.push_back(new FileExplorerPanel());
		m_Panels.push_back(new ScenePropertiesPanel());
		m_Panels.push_back(new LogPanel());
		m_Panels.push_back(new ConsolePanel());
	}

	EditorUI::~EditorUI() {
		for(auto panel : m_Panels) {
			delete panel;
		}
		m_Panels.clear();
	}

	void EditorUI::init() {
		// ImGui Fonts
		ImGuiIO& io = ImGui::GetIO();
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/OpenSans-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/OpenSans-Bold.ttf", 18.0f);

		// Editor Gui Initlialization
		EditorGui::EditorStyle::s_Instance = new EditorGui::EditorStyle();
		EditorGui::EditorStyle::setEditorStyle(EditorGui::EditorUIStyle::VisualStudio);
		EditorGui::EditorStyle::loadEditorIcons("assets/textures/editor_icons.png");
		EditorGui::EditorStyle::loadFileIcons("assets/textures/file_explorer_icons_32.png");

		for (auto panel : m_Panels) panel->init();
	}

	void EditorUI::loadPanelSettings() {
		for (auto panel : m_Panels) panel->loadSettings();
	}

	void EditorUI::setSceneContext(Ref<Scene> scene) {
		for (auto panel : m_Panels) panel->setContext(scene);
	}

	void EditorUI::onImGuiRender() {
		static bool p_open = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen) {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New Scene", "Ctrl+N")) {}
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) {}
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) {}
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Alt+F4")) { Application::get().close(); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Editor settings")) { m_EditorSettingsWindow.showWindow() = true; }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {

				for (auto panel : m_Panels) ImGui::MenuItem(panel->getPanelName().c_str(), NULL, panel->getPanelStatus());

				//ImGui::MenuItem("Scene Hierarchy", NULL, &m_ShowSceneHierarchyPanel);
				//ImGui::MenuItem("Properties", NULL, &m_ShowScenePropertiesPanel);
				//ImGui::MenuItem("Material Explorer", NULL, &m_ShowMaterialExplorerPanel);
				//ImGui::MenuItem("File Explorer", NULL, &m_ShowFileExplorerPanel);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		for (auto panel : m_Panels) panel->renderPanel();
		//if (m_ShowSceneHierarchyPanel) m_SceneHierarchyPanel.onImGuiRender();
		//if (m_ShowScenePropertiesPanel) m_ScenePropertiesPanel.onImGuiRender();
		//if (m_ShowMaterialExplorerPanel) m_MaterialExplorerPanel.onImGuiRender();
		//if (m_ShowFileExplorerPanel) m_FileExplorerPanel.onImGuiRender();

		uint32_t iconsTextureId = EditorGui::EditorStyle::getEditorIcons()->getRendererId();

		ImGui::Begin("Runtime");
		ImVec2 runtimeButtonSize = { 64.0f, 64.0f };
		ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 32.0f, 32.0f }, ImVec2(0.125f, 0.0f), ImVec2(0.25f, 0.125f), 8);
		ImGui::SameLine();
		ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 32.0f, 32.0f }, ImVec2(0.25f, 0.0f), ImVec2(0.375f, 0.125f), 8);
		ImGui::SameLine();
		ImGui::ImageButton((void*)iconsTextureId, ImVec2{ 32.0f, 32.0f }, ImVec2(0.0f, 0.0f), ImVec2(0.125f, 0.125f), 8);
		ImGui::End();

		bool status_bar = true;
		ImGui::PushStyleColor(ImGuiCol_ChildBg, EditorStyle::s_Instance->m_StatusBarBgColor);
		ImGuiWindowFlags statusBarFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoInputs;
		ImGui::Begin("Status Bar", &status_bar, statusBarFlags);

		ImGui::End();
		ImGui::PopStyleColor();

		ImGui::Begin("Info");

		ImGui::Text("Hovered Entity: ");
		ImGui::SameLine();
		if (m_HoveredEntity) {
			TagComponent tag = m_HoveredEntity.getComponent<TagComponent>();
			ImGui::Text(tag.Tag.c_str());
		}
		else {
			ImGui::Text("None");
		}

		ImGui::Text("Gizmo Type: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_GizmoType).c_str());

		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_CurrentFrames).c_str());

		ImGui::End(); // Settings end

		/*ImGui::Begin("Postprocessing test");
		ImGui::Text("Postprocessing: ");
		//uint32_t ppRenderId = m_PostProcessingOutline.getFramebuffer()->getColorAttachment(0);
		uint32_t ppRenderId = m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0);
		ImGui::Image((void*)ppRenderId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });
		ImGui::End();*/
		bool sameline = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		auto mousePos = ImGui::GetMousePos();
		auto windowPos = ImGui::GetWindowPos();
		auto windowSize = ImGui::GetWindowSize();
		auto contentRegion = ImGui::GetContentRegionAvail();

		float horizontalWaste = windowSize.x - contentRegion.x;
		float verticalWaste = windowSize.y - contentRegion.y;

		int normalizeMousePosX = mousePos.x - windowPos.x - horizontalWaste;
		int normalizeMousePosY = mousePos.y - windowPos.y - verticalWaste;

		m_ViewportMouseX = normalizeMousePosX;
		m_ViewportMouseY = contentRegion.y - normalizeMousePosY - 1;

		m_ViewportWidth = contentRegion.x;
		m_ViewportHeight = contentRegion.y;

		m_IsViewportFocused = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();

		Application::variables().Flag_ImGuiBlockEvents = (!m_IsViewportFocused && !m_IsViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
			m_ResizeHandler();
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		}

		//uint32_t viewportFramebufferRenderId = m_Layer->m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0);
		ImGui::Image((void*)m_ViewportRenderId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		//m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		// Gizmo
		if (EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
			EntitySelectionContext* context = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
			Entity selectedEntity = context->getSelectedEntity(); // TODO: central selected entity
			if (selectedEntity && m_GizmoType != -1) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				glm::mat4 viewMatrix = m_EditorCamera->getViewMatrix();
				glm::mat4 projectionMatrix = m_EditorCamera->getProjection();

				EntityTransform& et = selectedEntity.getComponent<TransformComponent>().Transform;
				glm::mat4 ettm = et.getWorldSpaceTransformationMatrix();

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(ettm));

				if (ImGuizmo::IsUsing()) {
					glm::vec3 translation{ 0.0f }, scale{ 1.0f };
					glm::quat orientation{};

					Math::decomposeTransform(ettm, translation, orientation, scale);
					et.set(translation, orientation, scale);
				}
			}
		}

		m_ImGuizmoIsHovering = ImGuizmo::IsOver();

		ImGui::End(); // Viewport end
		ImGui::PopStyleVar();

		ImGui::End(); // Docking end

		ImGui::ShowDemoWindow();

		if (m_EditorSettingsWindow.showWindow()) {
			ImGuiWindowFlags eswFlags = ImGuiWindowFlags_NoDocking;
			ImGui::Begin("Editor Settings", &m_EditorSettingsWindow.showWindow(), eswFlags);

			m_EditorSettingsWindow.onImGuiRender();

			ImGui::End();
		}

		if (m_EditorSettingsWindow.m_PreviousShowWindow && !m_EditorSettingsWindow.m_ShowWindow) {
			//EditorSettings::s_Instance->serializeSettings();
		}

		m_EditorSettingsWindow.m_PreviousShowWindow = m_EditorSettingsWindow.m_ShowWindow;
	}
}