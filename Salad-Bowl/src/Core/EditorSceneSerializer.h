#pragma once

#include "Salad/Scene/SceneSerializer.h"

#include "EditorScene.h"

namespace Salad {

	class EditorSceneSerializer : public SceneSerializer {
	
	public:
		EditorSceneSerializer(Scene* scene) : SceneSerializer(scene) {}
		~EditorSceneSerializer() = default;

		virtual void serializeScene(const std::string& filepath) override;
		virtual void deserializeScene(const std::string& filepath) override;
	};

}