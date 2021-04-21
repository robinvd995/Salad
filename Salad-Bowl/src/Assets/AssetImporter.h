#pragma once

#include "Asset.h"

namespace Salad::Asset::IO {

	TextureAsset loadTextureAsset(std::string& filepath);
	void saveTextureAsset(TextureAsset& texture);

	ShaderAsset loadShaderAsset(const std::string& filepath);
	void saveShaderAsset(ShaderAsset& shader);
}
