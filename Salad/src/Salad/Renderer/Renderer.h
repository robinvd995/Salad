#pragma once

#include "RenderAPI.h"
#include "OrthographicCamera.h"

#include "Shader.h"

namespace Salad {

	class Renderer {

	public:
		static void init();

		static void onWindowResized(uint32_t width, uint32_t height);

		static void beginScene(OrthographicCamera& camera);
		static void endScene();

		static void submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transformMat = glm::mat4(1.0f));

		inline static RenderAPI::API getAPI() { return RenderAPI::getAPI(); }

	private:
		struct SceneData {
			glm::mat4 viewProjectionMatrix;
		};

		static SceneData* m_SceneData;
	};

}