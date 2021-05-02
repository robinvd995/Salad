#pragma once

#include "Salad/Util/Stack.hpp"

#include "Salad/Renderer/Texture.h"

#include <map>

namespace Salad {

	// Change this to its own class to handle invalidation and resource relocation
	typedef uint32_t ResourceHandle;

	template<typename T>
	class ResourceCache {

	private:
		ResourceCache() = default;
		ResourceCache(const ResourceCache&) = delete;
		~ResourceCache() = default;

		void alloc(size_t size) {
			m_CacheSize = size;
			m_FreeHandleStack.alloc(size);
			for (uint32_t i = 0; i < size; i++) m_FreeHandleStack.push(size - i - 1);
			m_ResourcePtrArray = (T*)malloc(sizeof(T*) * size);
		}

		void freeCache() {
			free(m_ResourcePtrArray);
			m_FreeHandleStack.release();
		}

		// Store the resource, return the resource handle
		ResourceHandle storeResource(const std::string& resourceId, T resource) {
			ResourceHandle handle = m_FreeHandleStack.pop();
			m_ResourcePtrArray[handle] = resource;
			m_ResourceIdToHandleMap.insert({ resourceId, handle });
			return handle;
		}

		// Gets the resource by the resource handle
		T getResource(ResourceHandle resourceHandle) {
			return m_ResourcePtrArray[resourceHandle];
		}

		// Releases the resource, invalidating every handle that has been given out
		void releaseResource(ResourceHandle resourceHandle) {
			m_FreeHandleStack.push(resourceHandle);
			m_ResourcePtrArray[resourceHandle] = nullptr;
			
			for(auto it = m_ResourceIdToHandleMap.begin(); it != m_ResourceIdToHandleMap.end(); it++) {
				if (it->second == resourceHandle) m_ResourceIdToHandleMap.erase(it); break;
			}
		}

		bool hasResource(const std::string& resourceId, ResourceHandle* resourceHandle) {
			auto it = m_ResourceIdToHandleMap.find(resourceId);
			if (it == m_ResourceIdToHandleMap.end()) return false;
			*resourceHandle = it->second;
			return true;
		}

	private:
		size_t m_CacheSize = 0;

		std::map<const std::string, ResourceHandle> m_ResourceIdToHandleMap;
		T* m_ResourcePtrArray = nullptr;
		Util::Stack<ResourceHandle> m_FreeHandleStack;

		friend class ResourceManager;
	};

	class ResourceManager {
	
	public:
		ResourceManager() = delete;
		ResourceManager(const std::string& archiveLocation);
		~ResourceManager() = default;

		void init();
		void setupDefaultResources();
		void freeResources();

		static ResourceHandle allocateTexture(const std::string& resourceId) { return s_Instance->allocateTextureImpl(resourceId); }
		static void freeTexture(ResourceHandle handle) { s_Instance->freeTextureImpl(handle); }

		static Texture2D* getTexture(ResourceHandle handle) { return s_Instance->m_TextureCache.getResource(handle); }

		static const std::string& getResourceArchiveLocation() { return s_Instance->m_ResourceAcrhiveLocation; }

	private:
		inline static ResourceManager* s_Instance = nullptr;

		ResourceHandle allocateTextureImpl(const std::string& resourceId);
		void freeTextureImpl(ResourceHandle handle);

	private:
		std::string m_ResourceAcrhiveLocation;

		ResourceCache<Texture2D*> m_TextureCache;

		friend class ResourceLayer;
	};
}
