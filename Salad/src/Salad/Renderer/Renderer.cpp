#include "sldpch.h"
#include "Renderer.h"

#include "Renderer2D.h"

#include "RenderCommand.h"

namespace Salad {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::init() {
		RenderCommand::init();
		Renderer2D::init();
	}

	void Renderer::onWindowResized(uint32_t width, uint32_t height) {
		RenderCommand::setViewport(0, 0, width, height);
	}

	/*void Renderer::beginScene(Camera& camera) {
		m_SceneData->viewMatrix = camera.getViewMatrix();
		m_SceneData->projectionMatrix = camera.getProjectionMatrix();
	}*/

	void Renderer::beginScene(const RenderCamera& camera) {
		m_SceneData->viewMatrix = camera.view;
		m_SceneData->projectionMatrix = camera.projection;
	}

	void Renderer::endScene() {

	}

	void Renderer::submit(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const Transform& transform) {
		shader->bind();
		vertexArray->bind();
		shader->setMat4("u_Projection", m_SceneData->projectionMatrix);
		shader->setMat4("u_View", m_SceneData->viewMatrix);
		shader->setMat4("u_Transform", transform.getMatrix());

		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::submitEditorMesh(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray, const Transform& transform, int tag, bool selected) {
		shader->bind();
		vertexArray->bind();
		shader->setMat4("u_Projection", m_SceneData->projectionMatrix);
		shader->setMat4("u_View", m_SceneData->viewMatrix);
		shader->setMat4("u_Transform", transform.getMatrix());

		shader->setInt("u_Tag", tag);
		shader->setInt("u_Selected", selected ? 1 : 0);

		RenderCommand::drawIndexed(vertexArray);
	}

	void Renderer::submitSkybox(const Ref<Shader> shader, const Ref<VertexArray>& vertexArray) {

		glm::mat4 view = glm::mat4(glm::mat3(m_SceneData->viewMatrix));

		shader->bind();
		vertexArray->bind();
		shader->setMat4("u_Projection", m_SceneData->projectionMatrix);
		shader->setMat4("u_View", view);

		RenderCommand::drawTriangles(vertexArray, 36);
	}
}