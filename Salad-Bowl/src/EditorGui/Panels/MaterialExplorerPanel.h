#pragma once

#include "Salad/Renderer/TextureManager.h"
#include "EditorPanelBase.hpp"

#include <vector>
#include <cstdint>
#include <string>

namespace Salad::EditorGui {

	struct MaterialExplorerItem {

		std::string materialName;
		uint32_t materialViewId;

		MaterialExplorerItem(std::string& name, uint32_t viewid) : materialName(name), materialViewId(viewid) {}
	
	};

	class MaterialExplorerPanel : public EditorPanelBase {
	
	public:
		MaterialExplorerPanel() :
			EditorPanelBase("Materials", true)
		{
			m_MaterialTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/checkerboard.png");

			for(uint32_t i = 0; i < 10; i++) {
				std::string name = "Material ";
				name.append(std::to_string(i));
				m_MaterialItems.push_back({ name, i });
			}

			updateFilter();
		}

		virtual ~MaterialExplorerPanel() {}

		virtual void init() override {}
		virtual void loadSettings() override {}
		virtual void setContext(const Ref<Scene>& scene) override {}

	protected:
		virtual void onImGuiRender() override;

	private:
		void updateFilter();

	private:
		std::vector<MaterialExplorerItem> m_MaterialItems;
		std::vector<MaterialExplorerItem*> m_FilteredItems;

		Ref<Texture2D> m_MaterialTexture;

		std::string m_Filter;
		uint32_t m_Selected;
	};

}