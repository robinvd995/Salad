#include "EditorSettingsWindow.h"

#include "ImGui/imgui.h"
#include <stdio.h>

#include "Salad/ImGui/ImGuiWidgets.h"

namespace Salad {

	void drawPropertyList(EditorSettingsPropertyList& proplist) {
		for(auto& prop : proplist.properties) {
			switch (prop.type) {
				case EditorSettingsPropertyType::Float: ImGuiWidgets::drawFloatControl(prop.name.c_str(), (float*)prop.data, true, 0.0f); break;
				case EditorSettingsPropertyType::Flag: ImGuiWidgets::drawCheckboxControl(prop.name.c_str(), (bool*)prop.data); break;
				case EditorSettingsPropertyType::String: ImGuiWidgets::drawTextboxControl(prop.name.c_str(), (std::string*)prop.data); break;
			}
		}
	}

	void EditorSettingsWindow::onImGuiRender() {
		int height = ImGui::GetContentRegionAvail().y;
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("EditorSettingsSelector", ImVec2(200.0f, height), false, window_flags);
			ImGui::Dummy({ 0.0f, 2.0f });

			auto& groups = EditorSettings::s_Instance->m_Groups;

			for (auto& it = groups.begin(); it != groups.end(); it++) {
				if (ImGui::Selectable(it->groupId.c_str(), m_SelectedGroup == it->groupId)) {
					m_SelectedGroup = it->groupId;
				}
			}
			ImGui::EndChild();
		}
		
		ImGui::SameLine();

		if(!m_SelectedGroup.empty()) {
			ImGui::PushID(m_SelectedGroup.c_str());
			ImGuiWindowFlags window_flags = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 1.0f);
			ImGui::BeginChild("EditorSettingsProperties", ImVec2(0, height), true, window_flags);

			EditorSettingsGroup* group = EditorSettings::s_Instance->getGroupFromId(m_SelectedGroup);

			if (group != nullptr) {
				float prevColWidth = ImGuiWidgets::getColWidth();
				ImGuiWidgets::setColWidth(200.0f);

				for (auto it = group->subGroups.begin(); it != group->subGroups.end(); it++) {
					ImGui::PushID(it->first.c_str());
					if (ImGui::TreeNodeEx(it->first.c_str(), 0)) {
						EditorSettingsPropertyList& proplist = it->second;
						drawPropertyList(proplist);
						ImGui::TreePop();
					}
					ImGui::PopID();
				}
				drawPropertyList(group->propertyList);

				ImGuiWidgets::setColWidth(prevColWidth);
			}

			//ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::PopStyleVar();
			ImGui::PopID();
		}
	}

}
