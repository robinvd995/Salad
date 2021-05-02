#include "AssetBase.h"

#include "Salad/Core/Core.h"
#include "Salad/Core/Log.h"

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

	std::string assetFileFromPath(const std::string& filepath, bool createPath) {
		std::string prefix = ".editor/";
		std::string s = filepath;
		std::string fullpath = prefix.append(s.append(SLD_ASSET_FILE_EXTENSION));
		FileUtil::createDirectories(FileUtil::popFileName(fullpath));
		return fullpath;
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

	bool isAssetFile(const std::string& filepath) {
		std::string extension;
		bool hasExtension = FileUtil::fileExtensionFromName(filepath, &extension);
		return (hasExtension && (extension == "asset" || extension == ".asset"));
	}

	std::string assetSourceFromAssetFile(const std::string& filepath) {
		std::string s = filepath.substr(9);
		SLD_CORE_ASSERT(isAssetFile(s), "Asset is not a filepath");
		return FileUtil::popExtension(s);
	}
}
