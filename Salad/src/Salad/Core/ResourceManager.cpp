#include "sldpch.h"
#include "ResourceManager.h"

namespace Salad {

#define SLD_TEXTURE_CACHE_SIZE 512 // Memory size = SLD_TEXTURE_CACHE_SIZE * sizeof(T*) = 8
#define SLD_TEXTURE_MISSING_ID "defaults.texture_missing" // doesn't really matter, as long as it is unique and doesnt conflict with other resource id's

	void ResourceManager::init() {
		m_TextureCache.alloc(SLD_TEXTURE_CACHE_SIZE);
	}

	void ResourceManager::setupDefaultResources() {
		ResourceHandle defTextureHandle = m_TextureCache.storeResource(SLD_TEXTURE_MISSING_ID, Defaults::loadDefaultTexture2D());
		SLD_CORE_ASSERT((defTextureHandle == 0), "Default texture handle is expected to be 0!");
		// Setup default shader,  check if handle is 0, if it is not something went wrong
		// Setup default model,   check if handle is 0, if it is not something went wrong
		// Setup default etc.     check if handle is 0, if it is not something went wrong
	}

	void ResourceManager::freeResources() {
		m_TextureCache.freeCache();
	}

	ResourceHandle ResourceManager::allocateTextureImpl(const std::string& resourceId) {
		ResourceHandle handle = 0; // 0 SHOULD be missing texture resource handle
		if(m_TextureCache.hasResource(resourceId, &handle)) {
			return handle;
		}
		else{
			Texture2D* texture = loadTexture2D(resourceId.c_str());
			if(texture) handle = m_TextureCache.storeResource(resourceId, texture);
		}
		return handle;
	}

	void ResourceManager::freeTextureImpl(ResourceHandle handle) {
		m_TextureCache.releaseResource(handle);
	}
}
