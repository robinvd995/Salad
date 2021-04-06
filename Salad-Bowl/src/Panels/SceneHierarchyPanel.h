#pragma once

#include "Salad/Scene/Entity.h"
#include "Salad/Core/Core.h"
#include "Salad/Core/Log.h"
#include "Salad/Scene/Scene.h"

namespace Salad {

	class SceneHierarchyPanel {

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

		void setContext(const Ref<Scene>& scene);

		void onImGuiRender();

		Entity getSelectionContext() { return m_EntitySelectionContext; }
		bool hasSelectionContext() { return m_EntitySelectionContext.isValid(); }

	private:

		void drawEntityNode(Salad::Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_EntitySelectionContext;

		friend class Scene;
	};

}