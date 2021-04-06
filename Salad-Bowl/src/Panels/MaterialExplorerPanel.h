#pragma once

#include <vector>
#include <cstdint>
#include <string>

namespace Salad {

	struct MaterialExplorerItem {

		std::string materialName;

		MaterialExplorerItem(std::string& name) : materialName(name) {}
	
	};

	class MaterialExplorerPanel {
	
	public:
		MaterialExplorerPanel() {
			for(int i = 0; i < 10; i++) {
				std::string name = "Material ";
				name.append(std::to_string(i));
				m_MaterialItems.push_back({ name });
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
	};

}