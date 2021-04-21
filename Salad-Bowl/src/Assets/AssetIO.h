#pragma once

#include "AssetImporter.h"
#include "AssetExporter.h"

#include "Util/FileUtils.hpp"

namespace Salad::Asset::IO {

	inline std::string transformAssetIdToResourceId(std::string& assetid) {
		std::string sub_path = FileUtil::subPath(assetid, 1);
		std::string path_no_extension = FileUtil::popAllExtensions(sub_path);
		FileUtil::replaceFileSeperators(path_no_extension, '.');
		return path_no_extension;
	}

}