#include "AssetImporter.h"

#include "ShaderAssetBuilder.h"

#include "json.hpp"
#include <iomanip>
#include <fstream>
#include "Util/FileUtils.hpp";

using json = nlohmann::json;

namespace Salad::Asset {

/*#define SLD_ASSET_FILE_EXTENSION ".asset"

	std::string assetFileFromPath(std::string filepath) {
		return filepath.append(SLD_ASSET_FILE_EXTENSION);
	}

	TextureAsset Salad::Asset::loadTextureAsset(std::string& filepath) {
		TextureFilterWrapSpecification fwSpec;
		std::string assetFile = assetFileFromPath(filepath);

		if (!FileUtil::fileExists(assetFile)) {
			json j;

			j["MinFilter"] = static_cast<int>(fwSpec.minFilter);
			j["MagFilter"] = static_cast<int>(fwSpec.magFilter);
			j["WrapS"] = static_cast<int>(fwSpec.wrapS);
			j["WrapT"] = static_cast<int>(fwSpec.wrapT);
			j["WrapR"] = static_cast<int>(fwSpec.wrapR);

			std::ofstream file;
			file.open(assetFile);
			file << std::setw(4) << j;
			file.close();
		}
		else {
			std::ifstream ifs(assetFile);
			json jf = json::parse(ifs);

			fwSpec.minFilter = static_cast<TextureMinFilterSpecification>(jf["MinFilter"].get<int>());
			fwSpec.magFilter = static_cast<TextureMagFilterSpecification>(jf["MagFilter"].get<int>());
			fwSpec.wrapS = static_cast<TextureWrapSpecification>(jf["WrapS"].get<int>());
			fwSpec.wrapT = static_cast<TextureWrapSpecification>(jf["WrapT"].get<int>());
			fwSpec.wrapR = static_cast<TextureWrapSpecification>(jf["WrapR"].get<int>());

			ifs.close();
		}

		std::string filename = FileUtil::fileNameFromPath(filepath);
		Ref<Texture2D> texture = Texture2D::create(filepath, fwSpec);

		return TextureAsset(filepath, filename, texture, fwSpec);
	}

	void Salad::Asset::saveTextureAsset(TextureAsset& texture) {
		std::string assetFile = assetFileFromPath(texture.getFilePath());
		json j;

		j["MinFilter"] = static_cast<int>(texture.getTextureMinFilter());
		j["MagFilter"] = static_cast<int>(texture.getTextureMagFilter());
		j["WrapS"] = static_cast<int>(texture.getTextureWrapS());
		j["WrapT"] = static_cast<int>(texture.getTextureWrapT());
		j["WrapR"] = static_cast<int>(texture.getTextureWrapT());

		std::ofstream file;
		file.open(assetFile);
		file << std::setw(4) << j;
		file.close();
	}

	ShaderAsset Salad::Asset::loadShaderAsset(const std::string& filepath) {
		ShaderAssetBuilder builder;
		ShaderAsset& shader = builder.build(filepath);
		return shader;
	}
	
	void Salad::Asset::saveShaderAsset(ShaderAsset& shader) {

	}*/
}
