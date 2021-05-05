#include "Popups.h"

#include "imgui/imgui.h"
#include "Salad/ImGui/ImGuiWidgets.h"
#include "EditorAssetManager.h"

namespace Salad::EditorGui {

	void AssetExplorerPopup::show(Asset::AssetType assetType) {
		m_AssetType = assetType;
		m_AssetId = "";
		m_IsShowing = true;
		m_Confirmed = false;
		m_Selected = -1;

		ImGui::OpenPopup("AssetExplorer");
		m_AssetList.clear();
		EditorAssetManager::getAllAssetsOfType(assetType, m_AssetList);
	}

	bool AssetExplorerPopup::draw() {

		if (ImGui::BeginPopupModal("AssetExplorer", NULL, ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoResize)) {
			ImGuiWidgets::drawTextboxControl("Filter", &m_Filter);

			ImGui::BeginChild("AssetExplorerView", { 350.0f, 400.0f }, true);

			for(int i = 0; i < m_AssetList.size(); i++) {
				if(ImGui::Selectable(m_AssetList[i].c_str(), m_Selected == i)) {
					m_Selected = i;
					m_AssetId = m_AssetList[i];
				}
			}

			ImGui::EndChild();

			if (ImGui::Button("OK", ImVec2(120, 0))) { m_Confirmed = true; m_IsShowing = false; ImGui::CloseCurrentPopup(); }
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { m_IsShowing = false; ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}

		bool flag = false;
		if(m_LastIsShowing && !m_IsShowing && m_Confirmed) {
			flag = true;
		}
		m_LastIsShowing = m_IsShowing;
		m_Confirmed = false;
		return flag;

	}

	std::string& AssetExplorerPopup::getResult() { return m_AssetId; }
}
