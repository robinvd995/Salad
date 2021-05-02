#pragma once

#include "Salad/Core/Core.h"
#include "Scene.h"

namespace Salad {

	class SceneSerializer {
	
	public:
		SceneSerializer(Scene* scene) : m_Scene(scene) {}
		~SceneSerializer() {}

		virtual void serializeScene(const std::string& filepath);
		virtual void deserializeScene(const std::string& filepath);

	protected:
		entt::registry& getSceneEntityRegistry() { return m_Scene->m_Registry; }

	protected:
		Scene* m_Scene;
	};
}
