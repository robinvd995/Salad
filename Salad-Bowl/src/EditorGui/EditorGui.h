#pragma once

#include "Windows/EditorSettingsWindow.h"
#include "Core/EditorScene.h"
#include "Salad/Scene/Scene.h"
#include "Salad/Scene/Entity.h"
#include "Panels/EditorPanelBase.hpp"

#include "Core/EditorEventManager.hpp"

namespace Salad::EditorGui {

	class EditorUI : public EditorEventListener {
	
	public:

		//typedef void(*ViewportResizeHandler)(uint32_t viewportWidth, uint32_t viewportHeight);
		//typedef std::function<void(uint32_t, uint32_t)> ViewportResizeHandler;

		EditorUI();
		~EditorUI();

		void init();
		void loadPanelSettings();
		void setContext(EditorScene* context);
		//void setSceneContext(Ref<Scene> scene);

		void onImGuiRender();

		void setHoveredEntity(Entity entity) { m_HoveredEntity = entity; }
		void setCurrentFrames(uint32_t frames) { m_CurrentFrames = frames; }

		//void setViewportResizeHandler(std::function<void()> handler) { m_ResizeHandler = handler; }
		void setViewportRenderId(uint32_t viewportRenderId) { m_ViewportRenderId = viewportRenderId; }

		//void setEditorCamera(EditorCamera* camera) { m_EditorCamera = camera; }

		bool isViewportFocused() { return m_IsViewportFocused; }
		bool isViewportHovered() { return m_IsViewportHovered; }
		bool isViewportFocusedAndHovered() { return m_IsViewportFocused && m_IsViewportHovered; }
		bool isMouseInViewport() { return m_ViewportMouseX >= 0.0f && m_ViewportMouseY >= 0.0f && m_ViewportMouseX < m_ViewportWidth&& m_ViewportMouseY < m_ViewportHeight; }

		int getViewportMouseX() { return m_ViewportMouseX; }
		int getViewportMouseY() { return m_ViewportMouseY; }
		uint32_t getViewportWidth() { return m_ViewportWidth; }
		uint32_t getViewportHeight() { return m_ViewportHeight; }

		void setGizmoType(int type) { m_GizmoType = type; }
		bool isGizmoHovered() { return m_ImGuizmoIsHovering; }

		virtual void onEditorEvent(const EditorEvent& evnt) override;

	private:

		EditorScene* m_EditorContext = nullptr;

		// Panels new
		std::vector<EditorPanelBase*> m_Panels;

		EditorGui::EditorSettingsWindow m_EditorSettingsWindow;

		// Info panel
		Entity m_HoveredEntity;
		uint32_t m_CurrentFrames = 0;

		// viewport
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
		int m_ViewportMouseX = 0, m_ViewportMouseY = 0;
		int m_ViewportWidth = 0, m_ViewportHeight = 0;
		bool m_IsViewportFocused = false, m_IsViewportHovered = false;
		uint32_t m_ViewportRenderId = 0;

		//gizmo
		int m_GizmoType = -1;
		bool m_ImGuizmoIsHovering = false;
		EditorState m_EditorState;
	};

}