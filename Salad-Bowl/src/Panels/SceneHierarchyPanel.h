#pragma once

#include "Salad/Scene/Entity.h"
#include "Salad/Core/Core.h"
#include "Salad/Core/Log.h"
#include "Salad/Scene/Scene.h"

#include "EditorSelectionContext.h"

namespace Salad {

	class SceneHierarchyPanel {

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene, const Ref<EditorSelectionContext>& selection);

		void setContext(const Ref<Scene>& scene, const Ref<EditorSelectionContext>& selection);

		void onImGuiRender();

	private:

		void drawEntityNode(Salad::Entity entity);

	private:
		Ref<Scene> m_Context;
		Ref<EditorSelectionContext> m_SelectionContext;

		friend class Scene;
	};

}