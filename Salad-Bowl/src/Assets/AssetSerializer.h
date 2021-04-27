#pragma once

#include "TextureAsset.hpp"
#include "ShaderAsset.hpp"

namespace Salad::Asset {

#define SLD_ASSET_FILE_EXTENSION ".asset"

	class AssetSerializer {
	
	public:
		AssetSerializer() = default;
		~AssetSerializer() = default;

		void serializeTexture(TextureAsset& texture);
		TextureAsset deserializeTexture(const std::string& filepath);

		void serializeShader(ShaderAsset& shader);
		ShaderAsset deserializeShader(const std::string& filepath);

	private:
	
		void deserializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec);
		void serializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec);

		std::string assetFileFromPath(const std::string& filepath);

	};

}