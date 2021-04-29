#pragma once

#include "Assets/AssetManager.h"
#include "Util/FileObserver.h"

#include <thread>

namespace Salad {

	class EditorAssetManager {

	public:
		EditorAssetManager() = delete;
		EditorAssetManager(const EditorAssetManager&) = delete;
		EditorAssetManager(const std::string& assetOutput, const std::string& assetRegistry);
		~EditorAssetManager();

		static Asset::AssetManager& assetManager() { return s_Instance->m_AssetManager; }

		static void includeAsset(const std::string& filepath) { s_Instance->m_AssetManager.includeAsset(filepath); }
		static void excludeAsset(const std::string& filepath) { s_Instance->m_AssetManager.excludeAsset(filepath); }
		static void buildAsset(const std::string& filepath, bool forceBuild) { s_Instance->m_AssetManager.buildAsset(filepath, forceBuild); }
		static void buildAll(bool forceBuild) { s_Instance->m_AssetManager.buildAll(forceBuild); }
		static void clean() { s_Instance->m_AssetManager.clean(); }

	private:
		static void instantiate(const std::string& assetOutput, const std::string& assetRegisry);
		inline static EditorAssetManager* s_Instance;

		static void observe();
		static void onFileCreated(const std::string& filepath);
		static void onFileChanged(const std::string& filepath);
		static void onFileDeleted(const std::string& filepath);

		static void onAssetIncluded(const std::string& filepath);
		static void onAssetExcluded(const std::string& filepath);

		friend class EditorLayer;

	private:
		void serialize();
		void deserialize();

	private:
		Asset::AssetManager m_AssetManager;
		Util::FileObserver m_FileObserver;

		// File observer thead variables
		std::thread* m_FileObserverThread = nullptr;
		bool m_FileObserverThreadRunning = false;
	};
}
