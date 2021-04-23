#pragma once

#include "Asset.h"

namespace Salad::Asset::IO {

	//inline void ziplibExportTest();

	inline std::string transformAssetIdToResourceId(std::string& assetid);

	inline void exportTextureAsset(TextureAsset& asset);
}
