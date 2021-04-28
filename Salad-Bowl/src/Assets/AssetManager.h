#pragma once

#include "AssetBase.h"

#include "Salad/Util/Archiver.hpp"

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

		AssetData() = default;
		AssetData(bool p_dirty);
	};

	class AssetManager {
	
	public:
		//typedef void (*AssetExportFunc)(Util::ArchiveIOBuffer&, AssetBase*);

		AssetManager(const std::string& resourceOutput, const std::string assetManagerFile);
		AssetManager(const AssetManager&) = delete;
		~AssetManager() = default;

		void includeAsset(const std::string& filepath);
		void excludeAsset(const std::string& filepath);
		void buildAsset(const std::string& filepath, bool forceBuild);
		void buildAll(bool forceBuild);
		void clean();

		bool isAssetIncluded(const std::string& filepath);
		bool isAssetDirty(const std::string& filepath);

		AssetState getAssetState(const std::string& filepath);

		static bool isAsset(const std::string& filepath);

	private:
		void buildAssetInternal(const std::string& filepath, AssetData& data, bool forceBuild);
		void includeAsset(const std::string& filepath, AssetData data);
		std::string transformAssetIdToResourceId(const std::string& assetid);
	private:
		std::map<std::string, AssetData> m_IncludedAssets;
		//AssetExportFunc m_ExportFunctions[3];

		const std::string m_ResourceOutput;
		const std::string m_AssetManagerFile;

		friend class AssetManagerSerializer;
	};
}
