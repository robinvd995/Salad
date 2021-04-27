#include "AssetManager.h"

#include "Core.h"
#include "Util/FileUtils.hpp"

#include "AssetExporter.hpp"

namespace Salad::Asset {

	void dummyExportFunc(Util::ArchiveIOBuffer& buffer, AssetBase* asset) {
		SLD_BOWL_LOG_ERROR("Trying to export an asset with a not supported type!");
	}

	AssetManager::AssetManager() {
		m_ExportFunctions[static_cast<int>(AssetType::Unknown)] = dummyExportFunc;
		m_ExportFunctions[static_cast<int>(AssetType::Texture)] = exportTextureAsset;
		m_ExportFunctions[static_cast<int>(AssetType::Shader)] = exportShaderAsset;
	}

	std::string AssetManager::transformAssetIdToResourceId(std::string& assetid) {
		std::string sub_path = FileUtil::subPath(assetid, 1);
		std::string path_no_extension = FileUtil::popAllExtensions(sub_path);
		FileUtil::replaceFileSeperators(path_no_extension, '.');
		return path_no_extension;
	}

	void AssetManager::includeAsset(const std::string& filepath, AssetData data) {
		auto it = m_IncludedAssets.find(filepath);
		if (it != m_IncludedAssets.end()) return;
		m_IncludedAssets.insert({ filepath, data });
	}

	void AssetManager::includeAsset(const std::string& filepath) {
		includeAsset(filepath, AssetData());
	}

	void AssetManager::buildAsset(const std::string& filepath) {
		using namespace Util;
		auto it = m_IncludedAssets.find(filepath);
		if (it == m_IncludedAssets.end()) {
			SLD_BOWL_LOG_ERROR("Failed building asset '{0}', file is not included!", filepath);
			return;
		}

		int archiveError = 0;
		Archive archive = archiveOpen(SLD_ASSET_OUTPUT_ARCHIVE, ArchiverOpenFlags_Create, &archiveError);
		if(archive == NULL) {
			SLD_BOWL_LOG_ERROR(archiveErrorToString(archiveError));
			return;
		}


	}

	void AssetManager::buildAll() {}

	void AssetManager::rebuildAll() {}

	void AssetManager::clean() {}

}