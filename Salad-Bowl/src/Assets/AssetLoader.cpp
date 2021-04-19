#include "AssetLoader.h"

#include "json.hpp"
#include <iomanip>
#include <fstream>
#include "Util/FileUtils.hpp";

#define SLD_ASSET_FILE_EXTENSION ".asset"

using json = nlohmann::json;

namespace Salad::Asset {

	std::string assetFileFromPath(std::string filepath) {
		return filepath.append(SLD_ASSET_FILE_EXTENSION);
	}

	EditorTexture Salad::Asset::loadEditorTexture(std::string& filepath) {
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

		return EditorTexture(filepath, filename, texture, fwSpec);
	}

	void Salad::Asset::saveEditorTexture(EditorTexture& texture) {
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

}