#include "ResourceLayer.h"

#include <iostream>

namespace Salad {

	ResourceLayer::ResourceLayer() {
	
	}

	ResourceLayer::~ResourceLayer() {
	
	}

	void ResourceLayer::onAttach() {
		std::cout << "ResourceLayer attached!" << std::endl;

	}

	void ResourceLayer::onDetach() {

	}

	void ResourceLayer::onUpdate(Timestep ts) {

	}

	void ResourceLayer::onImGuiRender() {

	}

	void ResourceLayer::onEvent(Event& event) {

	}

}