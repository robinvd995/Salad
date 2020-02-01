#include "Salad.h"
#include <Salad/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <Salad/OrthographicCameraController.h>

#include "Sandbox2D.h"

class ExampleLayer : public Salad::Layer {

public:
	ExampleLayer() :
		Layer("Example"),
		m_CameraController(1280.0f / 720.0f, true)
	{
		{
			m_VertexArray = Salad::VertexArray::create();
			m_VertexArray->bind();

			float vertices[3 * 7] = {
				-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.5, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f
			};

			Salad::Ref<Salad::VertexBuffer> vertexBuffer = Salad::VertexBuffer::create(vertices, sizeof(vertices));

			Salad::BufferLayout layout = {
				{"a_Position", Salad::ShaderDataType::Float3},
				{"a_Color", Salad::ShaderDataType::Float4}
			};
			vertexBuffer->setLayout(layout);
			m_VertexArray->addVertexBuffer(vertexBuffer);

			unsigned int indices[3] = {
				0,1,2
			};

			Salad::Ref<Salad::IndexBuffer> indexBuffer = Salad::IndexBuffer::create(indices, 3);
			m_VertexArray->setIndexBuffer(indexBuffer);
			m_VertexArray->unbind();

			std::string vertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec4 a_Color;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec4 v_Color;

				void main(){
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
					v_Color = a_Color;
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core
			
				out vec4 frag_color;
				in vec4 v_Color;

				void main(){
				
					frag_color = v_Color;
				}
		
			)";

			m_Shader = Salad::Shader::create("shader0", vertexSrc, fragmentSrc);
		}

		{
			m_SqVertexArray = Salad::VertexArray::create();
			m_SqVertexArray->bind();

			float vertices[4 * 5] = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				0.5, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
				-0.5f, 0.5f, 0.0f, 0.0f, 1.0f
			};

			Salad::Ref<Salad::VertexBuffer> vertexBuffer = Salad::VertexBuffer::create(vertices, sizeof(vertices));

			Salad::BufferLayout layout = {
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 }
			};
			vertexBuffer->setLayout(layout);
			m_SqVertexArray->addVertexBuffer(vertexBuffer);

			unsigned int indices[6] = {
				0,1,2,3,0,2
			};

			Salad::Ref<Salad::IndexBuffer> indexBuffer = Salad::IndexBuffer::create(indices, 6);
			m_SqVertexArray->setIndexBuffer(indexBuffer);
			m_SqVertexArray->unbind();

			std::string vs = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec2 a_TexCoord;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				void main(){
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
				}
			)";

			std::string fs = R"(
				#version 330 core
			
				out vec4 frag_color;

				uniform vec3 u_Color;

				void main(){
				
					frag_color = vec4(u_Color, 1.0);
				}
		
			)";

			m_SqShader = Salad::Shader::create("shader1", vs, fs);
		}

		{
			std::string vertexSrc = R"(
				#version 330 core
			
				layout(location = 0) in vec3 a_Position;
				layout(location = 1) in vec2 a_TexCoord;

				uniform mat4 u_ViewProjection;
				uniform mat4 u_Transform;

				out vec2 v_TexCoord;				

				void main(){
					gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
					v_TexCoord = a_TexCoord;
				}
			)";

			std::string fragmentSrc = R"(
				#version 330 core

				out vec4 frag_color;
			
				in vec2 v_TexCoord;

				uniform sampler2D u_Texture;

				void main(){
				
					frag_color = texture(u_Texture, v_TexCoord);
				}
		
			)";

			auto shader = m_ShaderLibrary.load("assets/shaders/Texture.glsl");
		}

		m_Texture = Salad::Texture2D::create("assets/textures/checkerboard.png");
		m_Logo = Salad::Texture2D::create("assets/textures/logo.png");

		auto textureShader = m_ShaderLibrary.get("Texture");
		std::dynamic_pointer_cast<Salad::OpenGLShader>(textureShader)->bind();
		std::dynamic_pointer_cast<Salad::OpenGLShader>(textureShader)->setInt("u_Texture", 0);
	}

	void onUpdate(Salad::Timestep ts) override {
		
		//SLD_TRACE("Delta Time: {0}s {1}ms {2}fps", ts.getSeconds(), ts.getMilis(), 1.0f / ts.getSeconds());

		m_CameraController.onUpdate(ts);

		Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
		Salad::RenderCommand::clear();

		Salad::Renderer::beginScene(m_CameraController.getCamera());

		glm::vec3 scale(0.1f);

		std::dynamic_pointer_cast<Salad::OpenGLShader>(m_SqShader)->bind();
		std::dynamic_pointer_cast<Salad::OpenGLShader>(m_SqShader)->setFloat3("u_Color", m_SqColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.11f);

				glm::mat4 transform(1.0f);
				transform = glm::translate(transform, pos);
				transform = glm::scale(transform, scale);

				Salad::Renderer::submit(m_SqShader, m_SqVertexArray, transform);
			}
		}

		glm::mat4 transform(1.0f);
		glm::vec3 pos(0.0f);
		transform = glm::translate(transform, pos);
		transform = glm::scale(transform, glm::vec3(1.0f));

		//m_Texture->bind();
		//Salad::Renderer::submit(m_TextureShader, m_SqVertexArray, transform);

		m_Logo->bind();
		Salad::Renderer::submit(m_ShaderLibrary.get("Texture"), m_SqVertexArray, transform);

		Salad::Renderer::endScene();

	}

	virtual void onImGuiRender() override {
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SqColor));
		ImGui::End();
	}

	void onEvent(Salad::Event& e) override {
		m_CameraController.onEvent(e);

		Salad::EventDispatcher dispatcher(e);
		dispatcher.dispatch<Salad::KeyPressedEvent>(SLD_BIND_EVENT_FN(ExampleLayer::onKeyPressedEvent));
	}

	bool onKeyPressedEvent(Salad::KeyPressedEvent& e) {
		return false;
	}

private:
	Salad::ShaderLibrary m_ShaderLibrary;
	Salad::Ref<Salad::Shader> m_Shader;
	Salad::Ref<Salad::VertexArray> m_VertexArray;

	Salad::Ref<Salad::Shader> m_SqShader/*, m_TextureShader*/;
	Salad::Ref<Salad::VertexArray> m_SqVertexArray;

	Salad::Ref<Salad::Texture2D> m_Texture;
	Salad::Ref<Salad::Texture2D> m_Logo;

	Salad::OrthographicCameraController m_CameraController;

	glm::vec3 m_SqColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Salad::Application {
public:
	Sandbox() {
		//pushLayer(new ExampleLayer());
		pushLayer(new Sandbox2D());
	}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}
