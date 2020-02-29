#include "sldpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLShader.h"

namespace Salad {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::init() {
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResized(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}

	void Renderer::beginScene(Camera& camera) {
		m_SceneData->viewProjectionMatrix = camera.getViewProjectionMatrix();
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transformMat) {
		shader->bind();
		vertexArray->bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_ViewProjection", m_SceneData->viewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->setMat4("u_Transform", transformMat);

		RenderCommand::drawIndexed(vertexArray);
	}
}