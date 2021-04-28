#include "AssetBase.h"

#include "Util/FileUtils.hpp"

namespace Salad::Asset {

	AssetTypeData::AssetTypeData(const std::string& p_fileExtension) : m_FileExtension(p_fileExtension) {}

	const std::string& AssetTypeData::getFileExtension() const { return m_FileExtension; }

	const std::map<AssetType, AssetTypeData>  Salad::Asset::assetTypeDataMap = {
		{ AssetType::Unknown, AssetTypeData{ " "} },
		{ AssetType::Texture, AssetTypeData{ "png"} },
		{ AssetType::Shader, AssetTypeData{ "glsl" } },
		{ AssetType::Model, AssetTypeData{ "dae" } }
	};

	std::string assetFileFromPath(const std::string& filepath) {
		std::string s = filepath;
		return s.append(SLD_ASSET_FILE_EXTENSION);
	}

	AssetType assetTypeFromFilepath(const std::string& filepath) {
		std::string extension;
		bool hasExtension = FileUtil::fileExtensionFromName(filepath, &extension);
		if (hasExtension) {
			for(auto it = assetTypeDataMap.begin(); it != assetTypeDataMap.end(); it++) {
				if (it->second.getFileExtension() == extension) return it->first;
			}
			return AssetType::Unknown;
		}
		else {
			return AssetType::Unknown;
		}
	}
}
