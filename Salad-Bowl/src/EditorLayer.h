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

		bool onMouseScrolledEvent(MouseScrolledEvent& e);
		bool onKeyPressedEvent(KeyPressedEvent& e);

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
		glm::mat4 m_GizmoTransform{ 1.0f };
	};

}
