#include "AssetManager.h"

#include "Core/Core.h"
#include "Util/FileUtils.hpp"

#include "AssetExporter.hpp"
#include "AssetSerializer.h"

namespace Salad::Asset {

	AssetData::AssetData(bool p_dirty) : dirty(p_dirty) {}

	void dummyExportFunc(Util::ArchiveIOBuffer& buffer, AssetBase* asset) {
		SLD_BOWL_LOG_ERROR("Trying to export an asset with a not supported type!");
	}

	AssetManager::AssetManager(const std::string& resourceOutput, const std::string assetManagerFile) :
		m_ResourceOutput(resourceOutput), m_AssetManagerFile(assetManagerFile)
	{
		//m_ExportFunctions[static_cast<int>(AssetType::Unknown)] = dummyExportFunc;
		//m_ExportFunctions[static_cast<int>(AssetType::Texture)] = exportTextureAsset;
		//m_ExportFunctions[static_cast<int>(AssetType::Shader)] = exportShaderAsset;
	}

	std::string AssetManager::transformAssetIdToResourceId(const std::string& assetid) {
		std::string sub_path = FileUtil::subPath(assetid, 1);
		std::string path_no_extension = FileUtil::popAllExtensions(sub_path);
		FileUtil::replaceFileSeperators(path_no_extension, '.');
		return path_no_extension;
	}

	bool AssetManager::includeAsset(const std::string& filepath, bool notifySubscribers) {
		bool result = includeAssetInternal(filepath, AssetData());
		if (result && notifySubscribers) informSubscribersInclude(filepath);
		return result;
	}

	bool AssetManager::includeAsset(const std::string& filepath, AssetData& data, bool notifySubscribers) {
		bool result = includeAssetInternal(filepath, data);
		if (result && notifySubscribers) informSubscribersInclude(filepath);
		return result;
	}

	bool AssetManager::excludeAsset(const std::string& filepath, bool notifySubscribers) {
		bool result = excludeAssetInternal(filepath);
		if (result && notifySubscribers) informSubscribersExclude(filepath);
		return result;
	}

	bool AssetManager::buildAsset(const std::string& filepath, bool forceBuild) {

		// Check if asset is included
		auto it = m_IncludedAssets.find(filepath);
		if (it == m_IncludedAssets.end()) {
			SLD_BOWL_LOG_ERROR("Failed building asset '{0}', file is not included!", filepath);
			return false;
		}

		return buildAssetInternal(filepath, it->second, forceBuild);
	}

	bool AssetManager::buildAll(bool forceBuild) {
		SLD_BOWL_LOG_INFO("Started building assets, forced='{0}'.", forceBuild);
		for (auto it = m_IncludedAssets.begin(); it != m_IncludedAssets.end(); it++) {
			buildAssetInternal(it->first, it->second, forceBuild);
		}
		return true;
	}

	bool AssetManager::clean() {
		int error = remove(m_AssetManagerFile.c_str());
		if(error != 0) {
			SLD_BOWL_LOG_ERROR("An error occured while trying to clean the asset output!");
			return false;
		}

		for(auto it = m_IncludedAssets.begin(); it != m_IncludedAssets.end(); it++) {
			it->second.dirty = true;
		}
		return true;
	}

	bool AssetManager::buildAssetInternal(const std::string& filepath, AssetData& data, bool forceBuild) {
		using namespace Util;

		if (!forceBuild && !data.dirty) {
			SLD_BOWL_LOG_INFO("Asset is up to date '{0}', skipping build", filepath);
			return false;
		}

		// Check if the source file exists
		if (!FileUtil::fileExists(filepath)) {
			SLD_BOWL_LOG_ERROR("Failed building asset '{0}', source file does not exist!", filepath);
			return false;
		}

		// Check if the asset file exists
		if (!FileUtil::fileExists(assetFileFromPath(filepath, false))) {
			SLD_BOWL_LOG_ERROR("Failed building asset '{0}', asset file does not exist!", filepath);
			return false;
		}

		// Check if the file type is supported
		AssetType assetType = assetTypeFromFilepath(filepath);
		if (assetType == AssetType::Unknown) {
			SLD_BOWL_LOG_ERROR("Failed building asset '{0}', filetype is not supported!", filepath);
			return false;
		}

		int archiveError = 0;
		Archive archive = archiveOpen(m_ResourceOutput.c_str(), ArchiverOpenFlags_Create, &archiveError);
		if (archive == NULL) {
			SLD_BOWL_LOG_ERROR(archiveErrorToString(static_cast<ArchiverErrors_>(archiveError)));
			return false;
		}

		AssetSerializer serializer;
		ArchiveIOBuffer* buffer = nullptr;
		switch (assetType) {
			case AssetType::Texture:
			{
				TextureAsset asset = serializer.deserializeTexture(filepath);
				buffer = &archiveWriteBuffer(archive, asset.calculateAssetSize());
				exportTextureAsset(*buffer, &asset);
			} break;

			case AssetType::Shader:
			{
				ShaderAsset asset = serializer.deserializeShader(filepath);
				buffer = &archiveWriteBuffer(archive, asset.calculateAssetSize());
				exportShaderAsset(*buffer, &asset);
			} break;
		}

		if (buffer) {
			std::string assetId = transformAssetIdToResourceId(filepath);
			archiveAddFile(archive, assetId.c_str(), *buffer, 0);
			archiveClose(archive);
			buffer->freeBuffer();
			data.dirty = false;
			SLD_BOWL_LOG_INFO("Succesfully build asset '{0}'!", assetId);
			return true;
		}
		else{
			SLD_BOWL_LOG_ERROR("An error has occured when writing the asset to the buffer!");
			return false;
		}
	}

	bool AssetManager::includeAssetInternal(const std::string& filepath, AssetData data) {
		auto it = m_IncludedAssets.find(filepath);
		if (it != m_IncludedAssets.end()) return false;
		m_IncludedAssets.insert({ filepath, data });
		return true;
	}

	bool AssetManager::excludeAssetInternal(const std::string& filepath) {
		auto it = m_IncludedAssets.find(filepath);
		if (it == m_IncludedAssets.end()) return false;
		m_IncludedAssets.erase(it);
		return true;
	}

	bool AssetManager::isAssetIncluded(const std::string& filepath) {
		AssetState state = getAssetState(filepath);
		return state != AssetState::NotIncluded && state != AssetState::Unknown;
	}

	bool AssetManager::isAssetDirty(const std::string& filepath) {
		return getAssetState(filepath) == AssetState::Dirty;
	}

	void AssetManager::markAssetDirty(const std::string& filepath) {
		auto it = m_IncludedAssets.find(filepath);
		if (it == m_IncludedAssets.end()) return;
		it->second.dirty = true;
	}

	AssetState AssetManager::getAssetState(const std::string& filepath) {
		auto it = m_IncludedAssets.find(filepath);
		if (it == m_IncludedAssets.end()) return AssetState::NotIncluded;
		if (it->second.dirty) return AssetState::Dirty;
		return AssetState::UpToDate;
	}

	bool AssetManager::isAsset(const std::string& filepath) {
		AssetType type = assetTypeFromFilepath(filepath);
		return type != AssetType::Unknown;
	}

	void AssetManager::subscribeToInclude(AssetEventSubscribeFunc function) {
		m_IncludeSubscriptions.push_back(function);
	}

	void AssetManager::subscribeToExclude(AssetEventSubscribeFunc function) {
		m_ExcludeSubscriptions.push_back(function);
	}

	void AssetManager::informSubscribersInclude(const std::string& filepath) {
		for(AssetEventSubscribeFunc func : m_IncludeSubscriptions) {
			func(filepath);
		}
	}

	void AssetManager::informSubscribersExclude(const std::string& filepath) {
		for (AssetEventSubscribeFunc func : m_ExcludeSubscriptions) {
			func(filepath);
		}
	}
}
