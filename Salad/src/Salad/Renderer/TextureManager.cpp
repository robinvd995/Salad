#include "sldpch.h"
#include "TextureManager.h"

namespace Salad{

	Ref<Texture2D> TextureManager::loadTexture2D(const std::string& filepath) {
		Ref<Texture2D> textureRef = Texture2D::create(filepath);
		m_Texture2DCollection[filepath] = textureRef;
		return textureRef;
	}

	Ref<Texture2D> TextureManager::loadTexture2D(const std::string& textureId, uint32_t width, uint32_t height) {
		Ref<Texture2D> textureRef = Texture2D::create(width, height);
		m_Texture2DCollection[textureId] = textureRef;
		return textureRef;
	}

	Ref<Texture2D> TextureManager::getTexture2D(const std::string& textureId) {
		return m_Texture2DCollection[textureId];
	}

}
