#pragma once

#include "Salad/Renderer/Texture.h"
#include <iostream>

namespace Salad::Asset {

	class TextureAsset {
	
	public:
		TextureAsset() = delete;
		TextureAsset(std::string& path, std::string& name, Ref<Texture2D> texture, TextureFilterWrapSpecification filterWrapSpec)
			: m_FilePath(path), m_FileName(name), m_Texture(texture), m_TextureFilterWrapSpec(filterWrapSpec)
		{}
		TextureAsset(const TextureAsset& other) = default;
		~TextureAsset() = default;

		Ref<Texture2D> loadTexture() { m_Texture = Texture2D::create(m_FilePath, m_TextureFilterWrapSpec); return m_Texture; }

		std::string& getFileName() { return m_FileName; }
		std::string& getFilePath() { return m_FilePath; }
		
		// Exposing Texture2D
		uint32_t getTextureRenderId() { return m_Texture->getRendererId(); }
		uint32_t getTextureWidth() { return m_Texture->getWidth(); }
		uint32_t getTextureHeight() { return m_Texture->getHeight(); }

		// Texture filter/wrapping settings
		TextureMinFilterSpecification getTextureMinFilter() { return m_TextureFilterWrapSpec.minFilter; }
		TextureMagFilterSpecification getTextureMagFilter() { return m_TextureFilterWrapSpec.magFilter; }

		TextureWrapSpecification getTextureWrapS() { return m_TextureFilterWrapSpec.wrapS; }
		TextureWrapSpecification getTextureWrapT() { return m_TextureFilterWrapSpec.wrapT; }
		TextureWrapSpecification getTextureWrapR() { return m_TextureFilterWrapSpec.wrapR; }

		void setTextureMinFilter(TextureMinFilterSpecification filter) { m_TextureFilterWrapSpec.minFilter = filter; }
		void setTextureMagFilter(TextureMagFilterSpecification filter) { m_TextureFilterWrapSpec.magFilter = filter; }

		void setTextureWrapS(TextureWrapSpecification wrap) { m_TextureFilterWrapSpec.wrapS = wrap; }
		void setTextureWrapT(TextureWrapSpecification wrap) { m_TextureFilterWrapSpec.wrapT = wrap; }
		void setTextureWrapR(TextureWrapSpecification wrap) { m_TextureFilterWrapSpec.wrapR = wrap; }

		TextureFilterWrapSpecification& getTextureFilterWrapSpecification() { return m_TextureFilterWrapSpec; }

		void updateTextureFilterWrapSpec() { m_Texture->updateFilterWrapSpecification(m_TextureFilterWrapSpec); }

	private:
		std::string m_FilePath;
		std::string m_FileName;
		Ref<Texture2D> m_Texture;

		TextureFilterWrapSpecification m_TextureFilterWrapSpec;
	};
}
