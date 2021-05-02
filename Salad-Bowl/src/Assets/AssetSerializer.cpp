#include "AssetSerializer.h"

#include "ShaderAssetBuilder.h"

#include "Util/FileUtils.hpp";
#include "Util/Xml.h"

namespace Salad::Asset {

	// ----- Texture Serialization -----

	TextureAsset AssetSerializer::deserializeTexture(const std::string& filepath) {
		TextureFilterWrapSpecification fwSpec;
		std::string assetFilepath = assetFileFromPath(filepath, false);

		if (!FileUtil::fileExists(assetFilepath)) {
			serializeTextureData(assetFilepath, fwSpec);
		}
		else {
			deserializeTextureData(assetFilepath, fwSpec);
		}

		std::string filename = FileUtil::fileNameFromPath(filepath);
		Ref<Texture2D> texture = Texture2D::create(filepath, fwSpec);

		return TextureAsset(filepath, filename, texture, fwSpec);
	}

	void AssetSerializer::serializeTexture(TextureAsset& texture) {
		serializeTextureData(assetFileFromPath(texture.getFilePath(), true), texture.getTextureFilterWrapSpecification());
	}

	void AssetSerializer::deserializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec) {
		Xml::XmlDocument doc = Xml::parseXmlFile(assetFilepath, "texture_data");

		Xml::XmlNode filterNode = doc.getRootNode().getFirstChild("filter");
		fwSpec.minFilter = static_cast<TextureMinFilterSpecification>(filterNode.getAttributeAsInt("minFilter"));
		fwSpec.magFilter = static_cast<TextureMagFilterSpecification>(filterNode.getAttributeAsInt("magFilter"));

		Xml::XmlNode wrapNode = doc.getRootNode().getFirstChild("wrap");
		fwSpec.wrapS = static_cast<TextureWrapSpecification>(wrapNode.getAttributeAsInt("wrapS"));
		fwSpec.wrapT = static_cast<TextureWrapSpecification>(wrapNode.getAttributeAsInt("wrapT"));
		fwSpec.wrapR = static_cast<TextureWrapSpecification>(wrapNode.getAttributeAsInt("wrapR"));

		doc.close();
	}

	void AssetSerializer::serializeTextureData(const std::string& assetFilepath, TextureFilterWrapSpecification& fwSpec) {
		Xml::XmlDocument doc = Xml::createDocument("texture_data");

		doc.appendNodeToRoot(doc.allocateNode("filter")
			.appendAttribute("minFilter", static_cast<int>(fwSpec.minFilter))
			.appendAttribute("magFilter", static_cast<int>(fwSpec.magFilter)));
		doc.appendNodeToRoot(doc.allocateNode("wrap")
			.appendAttribute("wrapS", static_cast<int>(fwSpec.wrapS))
			.appendAttribute("wrapT", static_cast<int>(fwSpec.wrapT))
			.appendAttribute("wrapR", static_cast<int>(fwSpec.wrapR)));

		std::ofstream file(assetFilepath);
		file << doc;
		file.close();
		doc.close();
	}

	// ----- Texture Serialization -----

	ShaderAsset AssetSerializer::deserializeShader(const std::string& filepath) {
		ShaderAssetBuilder builder;
		ShaderAsset shader = builder.build(filepath);
		return shader;
	}

	void AssetSerializer::serializeShader(ShaderAsset& shader) {

	}
}