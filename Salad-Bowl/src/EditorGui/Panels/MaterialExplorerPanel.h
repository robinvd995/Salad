#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Salad::EditorGui {

	struct MaterialExplorerItem {

		std::string materialName;
		uint32_t materialViewId;

		MaterialExplorerItem(std::string& name, uint32_t viewid) : materialName(name), materialViewId(viewid) {}
	
	};

	class MaterialExplorerPanel {
	
	public:
		MaterialExplorerPanel() {
			for(uint32_t i = 0; i < 10; i++) {
				std::string name = "Material ";
				name.append(std::to_string(i));
				m_MaterialItems.push_back({ name, i });
			}

			updateFilter();
		}

		virtual ~MaterialExplorerPanel() {}

		void onImGuiRender(uint32_t textureid);

	private:
		void updateFilter();

	private:
		std::vector<MaterialExplorerItem> m_MaterialItems;
		std::vector<MaterialExplorerItem*> m_FilteredItems;

		std::string m_Filter;
		uint32_t m_Selected;
	};

}