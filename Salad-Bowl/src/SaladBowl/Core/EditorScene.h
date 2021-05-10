#pragma once

#include <Salad.h>

#include "SaladBowl/Renderer/EditorCamera.h"
#include "EditorEventManager.hpp"

namespace Salad {

	namespace EditorGui { class SceneHierarchyPanel; }

	enum class EditorState {
		Editor, Runtime, RuntimePaused
	};

	class EditorScene : public Scene, public EditorEventListener {
	
	public:
		EditorScene() = default;
		~EditorScene() = default;

		void init();
		virtual void onUpdate(Timestep& ts) override;
		void cleanup();

		void startRuntime();
		void pauseRuntime();
		void stopRuntime();

		EditorCamera& getEditorCamera() { return m_EditorCamera; }
		EditorState getEditorState() { return m_EditorState; }

		void setViewportFocused(bool isFocused) { m_ViewportFocused = isFocused; }
		void setViewportHovered(bool isHovered) { m_ViewportHovered = isHovered; }

		virtual void onEditorEvent(const EditorEvent& evnt) override;

	protected:
		virtual bool beginRenderScene() override;
		virtual void endRenderScene() override;

		virtual void renderMeshes(Timestep ts) override;

		virtual void updateScripts(Timestep ts) override;
		virtual void updateTransforms(Timestep ts) override;

	private:
		EditorState m_EditorState = EditorState::Editor;

		EditorCamera m_EditorCamera;
		bool m_ViewportFocused = false, m_ViewportHovered = false;

		friend class EditorGui::SceneHierarchyPanel;
	};

}