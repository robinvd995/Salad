#pragma once

#include "AssetBase.h"

//#include "Util/FileObserver.h"
#include "Salad/Util/Archiver.hpp"

#include <string>
#include <set>
#include <map>

namespace Salad::Asset {

#define SLD_ASSET_OUTPUT_ARCHIVE "resources.zip"

	struct AssetData {
		bool dirty = true;

		AssetData() = default;
		AssetData(bool p_dirty) : dirty(p_dirty) {}
	};

	class AssetManager {
	
	public:
		typedef void (*AssetExportFunc)(Util::ArchiveIOBuffer&, AssetBase*);

		AssetManager();
		~AssetManager() = default;

		void includeAsset(const std::string& filepath);
		void buildAsset(const std::string& filepath);
		void buildAll();
		void rebuildAll();
		void clean();

	private:
		void includeAsset(const std::string& filepath, AssetData data);
		std::string transformAssetIdToResourceId(std::string& assetid);
	private:
		std::map<std::string, AssetData> m_IncludedAssets;
		AssetExportFunc m_ExportFunctions[3];

		friend class AssetManagerSerializer;
	};
}
