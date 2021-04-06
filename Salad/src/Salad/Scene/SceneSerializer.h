#pragma once

#include "Salad/Core/Core.h"
#include "Scene.h"

namespace Salad {

	class SceneSerializer {
	
	public:
		SceneSerializer(Ref<Scene> scene) : m_Scene(scene) {}
		~SceneSerializer() {}

		void serializeScene(const std::string& filepath);
		void deserializeScene(const std::string& filepath);

	private:
		Ref<Scene> m_Scene;
	};
}