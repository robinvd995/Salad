#pragma once

#include "RenderAPI.h"
#include "Camera.h"
#include "Buffer.h"

#include "Shader.h"
#include "Salad/Core/Transform.h"

namespace Salad {

	struct RenderCamera {
		glm::mat4 projection;
		glm::mat4 view;
	};

	class Renderer {

	public:
		static void init();

		static void onWindowResized(uint32_t width, uint32_t height);

		//static void beginScene(Camera& camera);
		static void beginScene(const RenderCamera& camera);
		static void endScene();

		static void submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const Transform& transform);
		static void submitEditorMesh(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const Transform& transform, int tag, bool selected);

		static void submitSkybox(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray);

		inline static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }

		static void registerUniformBuffer(const uint32_t binding, const uint32_t size);
		static UniformBuffer* getUniformBuffer(const uint32_t binding);

	private:
		struct SceneData {
			glm::mat4 viewMatrix;
			glm::mat4 projectionMatrix;
		};

		struct UniformBufferData {
			std::vector<UniformBuffer*> m_UniformBuffers;
		};

		static SceneData* m_SceneData;
		static UniformBufferData* s_UniformBufferData;
	};
}
