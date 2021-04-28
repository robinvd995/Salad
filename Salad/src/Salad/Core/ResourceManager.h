#pragma once

#include "ResourceCache.hpp"

#include "Salad/Renderer/TextureLoader.hpp"

namespace Salad {

	class ResourceManager {
	
	public:
		ResourceManager() = default;
		~ResourceManager() = default;

		void allocateTexture(const char* resourceId);

	private:
		ResourceCache<Texture2D> m_TextureCache;
	};
}
