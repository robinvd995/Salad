#pragma once

#include <cstdint>
#include <map>
#include <string>

namespace Salad::Asset {

#define SLD_ASSET_FILE_EXTENSION ".asset"

	enum class AssetType {
		Unknown = 0,
		Texture = 1,
		Shader = 2,
		Model = 3
	};

	class AssetTypeData {

	public:
		AssetTypeData(const std::string& p_fileExtension);

		const std::string& getFileExtension() const;

	private:
		const std::string m_FileExtension;
	};

	extern const std::map<AssetType, AssetTypeData> assetTypeDataMap;

	class AssetBase {
	
	public:
		virtual AssetType getAssetType() = 0;
		virtual uint64_t calculateAssetSize() = 0;
	};

	std::string assetFileFromPath(const std::string& filepath);
	AssetType assetTypeFromFilepath(const std::string& filepath);
	bool isAssetFile(const std::string& filepath);
	std::string assetSourceFromAssetFile(const std::string& filepath);

}
