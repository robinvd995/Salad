#pragma once

#include "TextureAsset.hpp"
#include "ShaderAsset.hpp"

namespace Salad::Asset {

	TextureAsset loadTextureAsset(std::string& filepath);
	void saveTextureAsset(TextureAsset& texture);

	ShaderAsset loadShaderAsset(const std::string& filepath);
	void saveShaderAsset(ShaderAsset& shader);
}