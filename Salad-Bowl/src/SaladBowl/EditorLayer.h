#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

#include "EditorGui/Panels/SceneHierarchyPanel.h"
#include "EditorGui/Panels/ScenePropertiesPanel.h"
#include "EditorGui/Panels/MaterialExplorerPanel.h"
#include "EditorGui/Panels/FileExplorerPanel.h"

#include "EditorGui/Windows/EditorSettingsWindow.h"

#include "EditorSelectionContext.h"

#include "Salad/Renderer/PostProcessing.h"

#include "EditorGui/EditorGui.h"
#include "Core/EditorScene.h"

namespace Salad {

	class EditorLayer : public Layer, public EditorEventListener {

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

		void onViewportResized();

		virtual void onEditorEvent(const EditorEvent& evnt) override;

	private:
		void serialize();
		void deserialize();

	private:
		EditorGui::EditorUI m_EditorUI;
		EditorScene m_EditorScene;

		Ref<VertexArray> m_SkyboxVAO;
		Ref<Shader> m_SkyboxShader;
		Ref<TextureCubeMap> m_SkyBoxTexture;

		Ref<Framebuffer> m_Framebuffer;

		Entity m_HoveredEntity;

		// Frame timer
		// Chrono stuff ooofff!
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

		friend class EditorGui::EditorUI;
	};

}
