#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ScenePropertiesPanel.h"
#include "Panels/MaterialExplorerPanel.h"
#include "Panels/FileExplorerPanel.h"

#include "Windows/EditorSettingsWindow.h"

#include "Io/ColladaLoader.h"

#include "EditorCamera.h"
#include "EditorSelectionContext.h"

#include "Salad/Renderer/PostProcessing.h"

namespace Salad {

	enum class EditorState {
		Editor, Runtime
	};

	class EditorLayer : public Layer {

	public:

		EditorLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(Timestep ts) override;
		virtual void onEvent(Event& e) override;

		virtual void onImGuiRender() override;

		bool canMousePick();

		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);
		bool onMousePressedEvent(MouseButtonPressedEvent& e);

	private:
		void serialize();
		void deserialize();

	private:

		Entity m_CubeEntity;
		Entity m_TreeEntity;
		Entity m_LeavesEntity;
		Entity m_PlatformEntity;
		Entity m_CameraEntity;

		Ref<VertexArray> m_SkyboxVAO;
		Ref<Shader> m_SkyboxShader;
		Ref<TextureCubeMap> m_SkyBoxTexture;

		Ref<Texture2D> m_MaterialTexture;

		Ref<Scene> m_Scene;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize;

		bool m_IsViewportFocused = false;
		bool m_IsViewportHovered = false;

		EditorCamera m_EditorCamera;
		EditorState m_EditorState = EditorState::Editor;

		// Editor Panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
		ScenePropertiesPanel m_ScenePropertiesPanel;
		MaterialExplorerPanel m_MaterialExplorerPanel;
		FileExplorerPanel m_FileExplorerPanel;

		bool m_ShowSceneHierarchyPanel = true;
		bool m_ShowScenePropertiesPanel = true;
		bool m_ShowMaterialExplorerPanel = true;
		bool m_ShowFileExplorerPanel = true;

		// Windows
		EditorSettingsWindow m_EditorSettingsWindow;

		float m_EditorCameraSensitivity = 0.5f;

		// Editor Button Textures
		Ref<Texture2D> m_TextureButtonPlay;
		Ref<Texture2D> m_TextureButtonPause;
		Ref<Texture2D> m_TextureButtonStop;

		int m_GizmoType = -1;
		//glm::mat4 m_GizmoTransform{ 1.0f };

		int m_ViewportMouseX = 0, m_ViewportMouseY = 0;
		int m_ViewportWidth = 0, m_ViewportHeight = 0;
		Entity m_HoveredEntity;
		bool m_ImGuizmoIsHovering = false;

		// Move to Editor Scene
		Ref<EditorSelectionContext> m_EditorSelectionContext;

		// Frame timer
		// Chrono stuff YUCK!
		std::chrono::steady_clock::time_point m_PrevFrameStartTime;
		double m_CurFrameTime = 0.0;
		uint32_t m_CurFrames = 0;
		uint32_t m_CountedFrames = 0;
		double m_CurFrameTimeClock = 0.0;

		// Post processing
		PostProcessingEffect m_PostProcessingOutline;
		PostProcessingComposer m_PostProcessingComposer;
		bool m_PostProcessingFramebufferResize = false;
		uint32_t m_PostProcessingFramebufferWidth = 1280, m_PostProcessingFramebufferHeight = 720;
	};

}

