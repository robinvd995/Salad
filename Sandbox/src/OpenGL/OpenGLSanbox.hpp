#pragma once

#include "OpenGlSandboxConstants.h"
#include "Salad/Core/Layer.h"

#include <glm/glm.hpp>

#include <glad/glad.h>
#include <glm/glm/gtc/type_ptr.hpp>

namespace Salad {

	class OpenGLSandbox : public Layer {

	public:
		OpenGLSandbox() : Layer("OpenGLSandbox") {}
		~OpenGLSandbox() = default;

		virtual void onAttach() override {
			Salad::RenderCommand::setClearColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			m_Camera.setPerspectiveProjection(PerspectiveCameraProperties());

			m_Cube = VertexArray::create();

			Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(Constants::vertexData, sizeof(Constants::vertexData), SLD_STATIC_DRAW);
			vertexBuffer->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal",   Salad::ShaderDataType::Float3 }
			});

			m_Cube->addVertexBuffer(vertexBuffer);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(Constants::indexData, sizeof(Constants::indexData));
			m_Cube->setIndexBuffer(indexBuffer);
			m_Cube->unbind();

			m_Shader = Shader::create("assets/shaders/FlatColor.glsl");
			m_Shader->bind();
			m_Shader->setFloat4("u_Color0", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			m_Shader->setFloat4("u_Color1", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			m_Shader->setFloat4("u_Color2", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			m_Shader->unbind();

			m_RedShader = Shader::create("assets/shaders/FlatColorRed.glsl");
			m_BlueShader = Shader::create("assets/shaders/FlatColorBlue.glsl");

			/*uint32_t redSceneBufferIndex = glGetUniformBlockIndex(m_RedShader->getProgramId(), "u_Scene");
			glUniformBlockBinding(m_RedShader->getProgramId(), redSceneBufferIndex, 0);

			uint32_t blueSceneBufferIndex = glGetUniformBlockIndex(m_BlueShader->getProgramId(), "u_Scene");
			glUniformBlockBinding(m_BlueShader->getProgramId(), blueSceneBufferIndex, 0);

			uint32_t redMaterialBufferIndex = glGetUniformBlockIndex(m_RedShader->getProgramId(), "u_Material");
			glUniformBlockBinding(m_RedShader->getProgramId(), redMaterialBufferIndex, 1);

			uint32_t blueMaterialBufferIndex = glGetUniformBlockIndex(m_BlueShader->getProgramId(), "u_Material");
			glUniformBlockBinding(m_BlueShader->getProgramId(), blueMaterialBufferIndex, 2);*/

			glGenBuffers(1, &m_SceneBufferId);
			glBindBuffer(GL_UNIFORM_BUFFER, m_SceneBufferId);
			glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_SceneBufferId, 0, sizeof(glm::mat4) * 2);

			/*glGenBuffers(1, &m_MaterialBufferId);
			glBindBuffer(GL_UNIFORM_BUFFER, m_BlueMaterialBufferId);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_BlueMaterialBufferId, 0, sizeof(glm::vec4));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glGenBuffers(1, &m_RedMaterialBufferId);
			glBindBuffer(GL_UNIFORM_BUFFER, m_RedMaterialBufferId);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, 2, m_RedMaterialBufferId, 0, sizeof(glm::vec4));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);*/

			
			glGenBuffers(1, &m_MaterialBufferId);
			glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialBufferId);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), NULL, GL_STATIC_DRAW);
			glBindBufferRange(GL_UNIFORM_BUFFER, 1, m_MaterialBufferId, 0, sizeof(glm::vec4));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}

		virtual void onDetach() override {}

		virtual void onUpdate(Timestep ts) override {
			Salad::RenderCommand::clear();
			
			timer += 0.005f;

			Transform cubeTransform;
			glm::mat4 camTransform = glm::mat4(1.0f);
			camTransform[3][2] = 10.0f;
			m_Camera.recalculateViewMatrix();

			Transform redTransform;
			redTransform.setPosition(-2.0f, 0.0f, 0.0f);
			redTransform.calculateTransformationMatrix();

			Transform blueTransform;
			blueTransform.setPosition(2.0f, 0.0f, 0.0f);
			blueTransform.calculateTransformationMatrix();

			glm::mat4 view = glm::inverse(camTransform);

			//m_Shader->setFloat4("u_Color0", glm::vec4(0.0f, sin(timer) * cos(timer * 1.5f), 0.0f, 1.0f));
			//m_Shader->setFloat4("u_Color1", glm::vec4(sin(timer), 0.0f, 0.0f, 1.0f));
			//m_Shader->setFloat4("u_Color2", glm::vec4(0.0f, 0.0f, cos(timer / 2.0f), 1.0f));

			glBindBuffer(GL_UNIFORM_BUFFER, m_SceneBufferId);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(m_Camera.getProjection()));
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialBufferId);
			glm::vec4 bluecolordata = { 0.0f, 0.0f, 0.5f + sin(timer + 3.14159f) * 0.5f, 1.0f };
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(bluecolordata));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			m_Cube->bind();

			m_BlueShader->bind();
			m_BlueShader->setMat4("u_Transform", blueTransform.getMatrix());
			Salad::RenderCommand::drawIndexed(m_Cube);

			glBindBuffer(GL_UNIFORM_BUFFER, m_MaterialBufferId);
			glm::vec4 redcolordata = { 0.5f + sin(timer) * 0.5f, 0.0f, 0.0f, 1.0f };
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), glm::value_ptr(redcolordata));
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			m_RedShader->bind();
			m_RedShader->setMat4("u_Transform", redTransform.getMatrix());
			Salad::RenderCommand::drawIndexed(m_Cube);
		}

		virtual void onImGuiRender() override {}

		virtual void onEvent(Event& event) override {}

	private:
		Camera m_Camera;
		Transform m_CameraTransform;
		Ref<VertexArray> m_Cube;
		Ref<Shader> m_Shader;
		Ref<Shader> m_RedShader;
		Ref<Shader> m_BlueShader;

		uint32_t m_SceneBufferId;

		uint32_t m_MaterialBufferId;
		//uint32_t m_BlueMaterialBufferId;

		float timer = 0.0f;
	};

}