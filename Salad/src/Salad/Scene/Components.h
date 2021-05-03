#pragma once

#include <glm/glm.hpp>

#include "Salad/Renderer/VertexArray.h"
#include "Salad/Renderer/Shader.h"
#include "Salad/Renderer/Texture.h"
#include "Salad/Renderer/Camera.h"
#include "Salad/Scene/ScriptableEntity.h"

#define MAX_CHILDREN_PER_ENTITY 16

namespace Salad {

	struct TransformComponent {

		EntityTransform Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const EntityTransform& transform) : Transform(transform) {}

		operator EntityTransform& () { return Transform; }
		operator const EntityTransform& () const { return Transform; }
	};

	struct TagComponent {

		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) : Tag(tag) {}
	};

	struct MeshComponent {
		std::string meshResourceId = "mesh";
		std::string shaderResourceId = "material";
		std::string textureResourceId = "texture";

		Ref<VertexArray> MeshVertexArray;
		Ref<Shader> MeshShader;
		Ref<Texture2D> MeshTexture;

		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
		MeshComponent(Ref<VertexArray> vertexArray, Ref<Shader> shader, Ref<Texture2D> texture) :
			MeshVertexArray(vertexArray), MeshShader(shader), MeshTexture(texture)
		{}
	};

	struct CameraComponent {
		
		Salad::Camera Camera;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Salad::Camera& camera) : Camera(camera) {}

	};

	struct NativeScriptComponent {

		ScriptableEntity* instance = nullptr;

		ScriptableEntity*(*instantiateScript)();
		void (*destroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind() {
			instantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			destroyScript = [](NativeScriptComponent* nsc) { delete nsc->instance; nsc->instance = nullptr; };
		}
	};

	struct RelationshipComponent {
	
		entt::entity parentid = entt::null;
		uint32_t hierarchyDepth = 0;
		std::vector<entt::entity> children;

		RelationshipComponent() = default;
		RelationshipComponent(const RelationshipComponent&) = default;
	};

}
