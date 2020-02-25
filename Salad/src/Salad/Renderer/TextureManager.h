#pragma once

#include <Salad.h>

namespace Salad {

	class TextureManager {

	public:

		static TextureManager& get() {
			static TextureManager instance;
			return instance;
		}

		Ref<Texture2D> loadTexture2D(const std::string& filepath);
		Ref<Texture2D> loadTexture2D(const std::string& textureId, uint32_t width, uint32_t height);

		Ref<Texture2D> getTexture2D(const std::string& textureId);

		TextureManager() {}
		TextureManager(const TextureManager& other) = delete;

	private:
		std::unordered_map<std::string, Ref<Texture2D>> m_Texture2DCollection;
	};

}