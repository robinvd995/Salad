#pragma once

#include "Assets/AssetManager.h"
#include "Util/FileObserver.h"

namespace Salad {

	class EditorAssetManager {

	public:
		EditorAssetManager() = delete;
		EditorAssetManager(const EditorAssetManager&) = delete;
		EditorAssetManager(const std::string& assetOutput, const std::string& assetRegistry);
		~EditorAssetManager();

		static Asset::AssetManager& assetManager() { return s_Instance->m_AssetManager; }

	private:
		inline static void instantiate(const std::string& assetOutput, const std::string& assetRegisry) { s_Instance = new EditorAssetManager(assetOutput, assetRegisry); }
		inline static EditorAssetManager* s_Instance;
		friend class EditorLayer;

	private:
		Asset::AssetManager m_AssetManager;

	};
}
