#pragma once

#include "Assets/TextureAsset.hpp"
#include "Assets/ShaderAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/MaterialAsset.hpp"

namespace Salad::Asset {

	class AssetSerializer {
	
	public:
		AssetSerializer() = default;
		~AssetSerializer() = default;

		void serializeTexture(TextureAsset& texture);
		TextureAsset deserializeTexture(const std::string& filepath);

		void serializeShader(ShaderAsset& shader);
		ShaderAsset deserializeShader(const std::string& filepath);

		void serializeModel(Ref<ModelAsset> model);
		Ref<ModelAsset> deserializeModel(const std::string& filepath);

		void serializeMaterial(Ref<MaterialAsset> material);
		Ref<MaterialAsset> deserializeMaterial(const std::string& filepath);

	private:
	
		void deserializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec);
		void serializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec);

		//std::string assetFileFromPath(const std::string& filepath);
	};
}
