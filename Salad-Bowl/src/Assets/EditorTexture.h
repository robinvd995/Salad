#pragma once

#include "Salad/Renderer/Texture.h"
#include <iostream>

namespace Salad {

	enum class TextureMinFilterStyle {
		Linear = 0,
		Nearest = 1,
		NearestMipmapNearest = 2,
		LinearMipmapNearest = 3,
		NearestMipmapLinear = 4,
		LinearMipmapLinear = 5
	};

	enum class TextureMagFilterStyle {
		Linear = 0,
		Nearest = 1
	};

	enum class TextureWrapStyle {
		Repeat = 0,
		ClampToEdge = 1,
		ClampToBorder = 2,
		MirroredRepeat = 3,
		MirroredClampToEdge = 4
	};

	class EditorTexture {
	
	public:
		EditorTexture() = delete;
		EditorTexture(std::string& path, std::string& name) : m_FilePath(path), m_FileName(name) {}
		EditorTexture(const EditorTexture& other) = default;
		~EditorTexture() = default;

		void loadTexture() { m_Texture = Texture2D::create(m_FilePath); }

		std::string& getFileName() { return m_FileName; }
		std::string& getFilePath() { return m_FilePath; }
		
		// Exposing Texture2D
		uint32_t getTextureRenderId() { return m_Texture->getRendererId(); }
		uint32_t getTextureWidth() { return m_Texture->getWidth(); }
		uint32_t getTextureHeight() { return m_Texture->getHeight(); }

		// Texture filter/wrapping settings
		TextureMinFilterStyle getTextureMinFilter() { return m_TextureMinFilter; }
		TextureMagFilterStyle getTextureMagFilter() { return m_TextureMagFilter; }

		TextureWrapStyle getTextureWrapS() { return m_TextureWrapS; }
		TextureWrapStyle getTextureWrapT() { return m_TextureWrapT; }
		TextureWrapStyle getTextureWrapR() { return m_TextureWrapR; }

		void setTextureMinFilter(TextureMinFilterStyle filter) { m_TextureMinFilter = filter; }
		void setTextureMagFilter(TextureMagFilterStyle filter) { m_TextureMagFilter = filter; }

		void setTextureWrapS(TextureWrapStyle wrap) { m_TextureWrapS = wrap; }
		void setTextureWrapT(TextureWrapStyle wrap) { m_TextureWrapT = wrap; }
		void setTextureWrapR(TextureWrapStyle wrap) { m_TextureWrapR = wrap; }

	private:
		std::string m_FilePath;
		std::string m_FileName;
		Ref<Texture2D> m_Texture;

		TextureMinFilterStyle m_TextureMinFilter = TextureMinFilterStyle::NearestMipmapLinear;
		TextureMagFilterStyle m_TextureMagFilter = TextureMagFilterStyle::Linear;

		TextureWrapStyle m_TextureWrapS = TextureWrapStyle::Repeat;
		TextureWrapStyle m_TextureWrapT = TextureWrapStyle::Repeat;
		TextureWrapStyle m_TextureWrapR = TextureWrapStyle::Repeat;
	};

}