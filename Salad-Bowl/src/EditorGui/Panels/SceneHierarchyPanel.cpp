#include "SceneHierarchyPanel.h"

#include "Salad/Scene/Components.h"

#include <imgui/imgui.h>

namespace Salad::EditorGui {

	/*SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene, const Ref<EditorSelectionContext>& selection) {
		setContext(scene);
	}*/

	void SceneHierarchyPanel::init() {

	}

	void SceneHierarchyPanel::loadSettings() {

	}

	void SceneHierarchyPanel::setContext(const Ref<Scene>& scene) {
		m_Context = scene;
	}

	void SceneHierarchyPanel::onImGuiRender() {

		//ImGui::Begin("Scene Hierarchy");

		m_Context->m_Registry.each([&](auto entityid) {
			drawEntityNode({ entityid, m_Context.get() });
		});

		if(ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
			m_SelectedEntity = {};
			EditorSelectionContext::setSelectionContextNone();
			//m_SelectionContext->setSelectionContext({});
		}

		if(ImGui::BeginPopupContextWindow(0, 1, false)) {
			if (ImGui::Selectable("Create Entity")) { m_Context->createEntity("Empty Entity"); }
			ImGui::EndPopup();
		}

		//ImGui::End();
	}

	void SceneHierarchyPanel::drawEntityNode(Salad::Entity entity) {
		TagComponent& tagc = entity.getComponent<TagComponent>();

		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)((uint32_t)entity), flags, tagc.Tag.c_str());

		if(ImGui::IsItemClicked()) {
			m_SelectedEntity = entity;
			EditorSelectionContext::setSelectionContext<EntitySelectionContext>(entity);
		}

		bool entityDeleted = false;
		if(ImGui::BeginPopupContextItem(0)) {
			if (ImGui::MenuItem("Delete Entity")) entityDeleted = true;
			ImGui::EndPopup();
		}

		if(opened) {
			ImGui::TreePop();
		}

		if (entityDeleted) {
			if (m_SelectedEntity == entity) { m_SelectedEntity = {}; EditorSelectionContext::setSelectionContextNone(); }
			m_Context->removeEntity(entity);
		}
	}

}