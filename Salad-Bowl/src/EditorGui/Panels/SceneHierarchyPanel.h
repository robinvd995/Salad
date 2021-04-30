#pragma once

#include "Salad/Scene/Entity.h"
#include "Salad/Core/Core.h"
#include "Salad/Core/Log.h"
#include "Salad/Scene/Scene.h"

#include "EditorSelectionContext.h"
#include "EditorPanelBase.hpp"

namespace Salad::EditorGui {

	class SceneHierarchyPanel : public EditorPanelBase {

	public:
		SceneHierarchyPanel() :
			EditorPanelBase("Scene Hierarchy", true) 
		{}

		virtual void init() override;
		virtual void loadSettings() override;
		virtual void setContext(const Ref<Scene>& scene) override;

	protected:
		virtual void onImGuiRender() override;

	private:

		void drawEntityNode(Salad::Entity entity);

	private:
		Ref<Scene> m_Context;
		Entity m_SelectedEntity;

		friend class Scene;
	};
}
