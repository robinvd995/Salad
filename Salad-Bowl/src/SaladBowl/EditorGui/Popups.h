#pragma once

#include "SaladBowl/Assets/Core/AssetBase.h"

#include <vector>

namespace Salad::EditorGui {

	class AssetExplorerPopup {
	
	public:
		AssetExplorerPopup() = default;
		~AssetExplorerPopup() = default;

		void show(Asset::AssetType assetType);
		bool draw();

		std::string& getResult();

	private:
		Asset::AssetType m_AssetType = Asset::AssetType::Unknown;
		std::string m_AssetId = "";
		bool m_IsShowing = false;
		bool m_LastIsShowing = false;
		bool m_Confirmed = false;

		std::string m_Filter;
		std::vector<std::string> m_AssetList;
		int m_Selected = -1;
	};
}
