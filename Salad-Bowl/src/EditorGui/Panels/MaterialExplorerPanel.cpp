#include "MaterialExplorerPanel.h"

#include <iostream>
#include <algorithm>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Salad::EditorGui {

    static char s_FilterBuffer[256];

	void MaterialExplorerPanel::onImGuiRender() {

        //ImGui::ShowDemoWindow();

        const float minColWidth = 128.0f;

		ImGui::Begin("Materials");

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, 80.0f);
        ImGui::Text("Filter");
        ImGui::NextColumn();

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 6.0f });

        memset(s_FilterBuffer, 0, sizeof(s_FilterBuffer));
        strcpy_s(s_FilterBuffer, sizeof(s_FilterBuffer), m_Filter.c_str());

        ImGuiIO& io = ImGui::GetIO();
        auto& boldFont = io.Fonts->Fonts[1];

        float lineHeight = boldFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;

        ImVec2 buttonSize = { lineHeight, lineHeight };

        float filterInputWidth = ImGui::GetContentRegionAvail().x - buttonSize.x;
        ImGui::PushItemWidth(filterInputWidth);
        if (ImGui::InputText("##Filter", s_FilterBuffer, sizeof(s_FilterBuffer))) {
            m_Filter = std::string(s_FilterBuffer);
            updateFilter();
        }
        ImGui::PopItemWidth();

        ImGui::SameLine();
        ImGui::PushFont(boldFont);
        if(ImGui::Button("X", buttonSize)) { m_Filter.clear(); updateFilter(); }
        ImGui::PopFont();

        ImGui::PopStyleVar();

        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::Dummy({ 0.0f, 2.0f });

        int height = ImGui::GetContentRegionAvail().y;
        ImGuiWindowFlags window_flags = 0;
        ImGui::BeginChild("MaterialExplorerView", ImVec2(0, height), false, window_flags);

        ImGuiStyle& style = ImGui::GetStyle();
        ImVec2 materialSize{ 80, 80 };
        float windowVisibleX2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        for (int n = 0; n < m_FilteredItems.size(); n++) {
            ImGui::PushID(n);
            MaterialExplorerItem* item = m_FilteredItems[n];

            // Draw material
            ImVec2 p = ImGui::GetCursorScreenPos();
            bool selected = m_Selected == item->materialViewId;
            if(ImGui::Selectable("##selectabletest", &selected, 0, ImVec2(100.0f, 120.0f))) {
                m_Selected = item->materialViewId;
            }
            float lastGroupX2 = ImGui::GetItemRectMax().x;
            float nextGroupX2 = lastGroupX2 + style.ItemSpacing.x + materialSize.x;
            if (n + 1 < m_FilteredItems.size() && nextGroupX2 < windowVisibleX2)
                ImGui::SameLine();
            //if (sameline) ImGui::SameLine();
            ImVec2 afterScreenPos = ImGui::GetCursorScreenPos();

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 10.0f });
            if (ImGui::BeginPopupContextItem(item->materialName.c_str(), ImGuiMouseButton_Right)) {
                ImGui::Text(item->materialName.c_str());
                ImGui::Separator();
                ImGui::Selectable("Edit");
                ImGui::Selectable("Delete");
                ImGui::EndPopup();
            }
            ImGui::PopStyleVar();

            ImGui::GetWindowDrawList()->AddImage((void*)m_MaterialTexture->getRendererId(), ImVec2(p.x + 4, p.y + 4), ImVec2(p.x + 96, p.y + 96), ImVec2(0, 0), ImVec2(1, 1));

            ImGui::SetCursorScreenPos(ImVec2(p.x + 4, p.y + 100));
            ImGui::PushItemWidth(120.0f);
            ImGui::BeginChild("testchildwindow", ImVec2(92.0f, 20.0f));
            ImGui::Text(item->materialName.c_str());
            ImGui::EndChild();
            ImGui::PopItemWidth();
            ImGui::SetCursorScreenPos(afterScreenPos);

            ImGui::PopID();
        }

        ImGui::EndChild();

		ImGui::End();

	}

    void MaterialExplorerPanel::updateFilter() {
    
        m_FilteredItems.clear();

        if(m_Filter.empty()) {
            m_FilteredItems.resize(m_MaterialItems.size());
            for (int i = 0; i < m_FilteredItems.size(); i++)
                m_FilteredItems[i] = &m_MaterialItems[i];
        }

        else {
            for (MaterialExplorerItem& item : m_MaterialItems) {

                if(item.materialName.find(m_Filter) != std::string::npos) {
                    m_FilteredItems.push_back(&item);
                } 
            }
        }
    }

}