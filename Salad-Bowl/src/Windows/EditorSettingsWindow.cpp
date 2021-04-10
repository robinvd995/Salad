#include "EditorSettingsWindow.h"

#include "ImGui/imgui.h"
#include <stdio.h>

#include "Salad/ImGui/ImGuiWidgets.h"

namespace Salad {

	void EditorSettingsWindow::onImGuiRender() {
		int height = ImGui::GetContentRegionAvail().y;
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_HorizontalScrollbar;
			ImGui::BeginChild("EditorSettingsSelector", ImVec2(200.0f, height), false, window_flags);
			ImGui::Dummy({ 0.0f, 2.0f });

			auto& group = m_Settings.m_PropertyMap;

			for (auto it = group.begin(); it != group.end(); it++) {
				if (ImGui::Selectable(it->first.c_str(), m_SelectedGroup == it->first)) {
					m_SelectedGroup = it->first;
				}

			}
			ImGui::EndChild();
		}

		ImGui::SameLine();

		if(!m_SelectedGroup.empty()) {
			ImGuiWindowFlags window_flags = 0;
			ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 1.0f);
			ImGui::BeginChild("EditorSettingsProperties", ImVec2(0, height), true, window_flags);

			float prevColWidth = ImGuiWidgets::getColWidth();
			ImGuiWidgets::setColWidth(200.0f);

			auto& properties = m_Settings.m_PropertyMap[m_SelectedGroup];
			for(auto& prop : properties) {
				switch(prop.type) {
					case PropertyType::Float: ImGuiWidgets::drawFloatControl(prop.name.c_str(), (float*)prop.data, true, 0.0f); break;
					// Float slider, maybe t case PropertyType::Float: ImGuiWidgets::drawFloatSlider(prop.name.c_str(), (float*)prop.data, 0.01f, 1.0f); break;
					case PropertyType::Flag: ImGuiWidgets::drawCheckboxControl(prop.name.c_str(), (bool*)prop.data); break;
					case PropertyType::Seperator: ImGui::Separator(); break;
					case PropertyType::Tag: ImGui::Text(prop.name.c_str()); break;
					case PropertyType::ID: ImGui::PushID(prop.name.c_str()); break;
					case PropertyType::PopID: ImGui::PopID(); break;
						
				}
			}

			ImGuiWidgets::setColWidth(prevColWidth);

			//ImGui::Columns(1);
			ImGui::EndChild();
			ImGui::PopStyleVar();
		}
	}

}
