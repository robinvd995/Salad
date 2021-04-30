#include "sldpch.h"
#include "ResourceLayer.h"

#include "ResourceManager.h"

namespace Salad {

	ResourceLayer::ResourceLayer() {

	}

	void ResourceLayer::onAttach() {
		ResourceManager::s_Instance = new ResourceManager();
		ResourceManager::s_Instance->init();
		ResourceManager::s_Instance->setupDefaultResources();
	}

	void ResourceLayer::onDetach() {
		ResourceManager::s_Instance->freeResources();
		delete ResourceManager::s_Instance;
		ResourceManager::s_Instance = nullptr;
	}

	void ResourceLayer::onUpdate(Timestep ts) {

	}

	void ResourceLayer::onEvent(Event& e) {

	}

	void ResourceLayer::onImGuiRender() {

	}
}
