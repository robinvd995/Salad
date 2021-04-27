#pragma once

#include <cstdint>

namespace Salad::Asset {

	enum class AssetType {
		Unknown = 0,
		Texture = 1,
		Shader = 2
	};

	class AssetBase {
	
	public:
		virtual AssetType getAssetType() = 0;
		virtual uint64_t calculateAssetSize() = 0;
	};

}
