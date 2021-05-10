#pragma once

#include "AssetBase.h"

#include "Salad/Util/Archiver.hpp"

#include <vector>
#include <string>
#include <set>
#include <map>

namespace Salad::Asset {

	enum class AssetState {
		Unknown = 0,
		UpToDate = 1,
		Dirty = 2,
		NotIncluded = 3
	};

	struct AssetData {
		bool dirty = true;
		AssetType type = AssetType::Unknown;

		AssetData() = delete;
		AssetData(bool p_dirty, AssetType p_type);
	};

	class AssetManager {
	
	public:
		//typedef void (*AssetExportFunc)(Util::ArchiveIOBuffer&, AssetBase*);
		typedef void (*AssetEventSubscribeFunc)(const std::string& filepath);

		AssetManager(const std::string& resourceOutput, const std::string assetManagerFile);
		AssetManager(const AssetManager&) = delete;
		~AssetManager() = default;

		bool includeAsset(const std::string& filepath, AssetType type, bool notifySubscribers = true);
		bool includeAsset(const std::string& filepath, AssetData& data, bool notifySubscribers = true);
		bool excludeAsset(const std::string& filepath, bool notifySubscribers = true);
		bool buildAsset(const std::string& filepath, bool forceBuild);
		bool buildAll(bool forceBuild);
		bool clean();

		bool isAssetIncluded(const std::string& filepath);
		bool isAssetDirty(const std::string& filepath);
		void markAssetDirty(const std::string& filepath);

		AssetState getAssetState(const std::string& filepath);
		static bool isAsset(const std::string& filepath);

		void subscribeToInclude(AssetEventSubscribeFunc function);
		void subscribeToExclude(AssetEventSubscribeFunc function);

		void getAllAssetsOfType(AssetType type, std::vector<std::string>& assetList);

	private:
		bool buildAssetInternal(const std::string& filepath, AssetData& data, bool forceBuild);
		bool includeAssetInternal(const std::string& filepath, AssetData data);
		bool excludeAssetInternal(const std::string& filepath);

		void informSubscribersInclude(const std::string& filepath);
		void informSubscribersExclude(const std::string& filepath);

		std::string transformAssetIdToResourceId(const std::string& assetid);
	private:
		std::map<std::string, AssetData> m_IncludedAssets;
		//AssetExportFunc m_ExportFunctions[3];

		std::vector<AssetEventSubscribeFunc> m_IncludeSubscriptions;
		std::vector<AssetEventSubscribeFunc> m_ExcludeSubscriptions;

		const std::string m_ResourceOutput;
		const std::string m_AssetManagerFile;

		friend class AssetManagerSerializer;
	};
}
