#include "sldpch.h"
#include "Renderer2D.h"

#include "Salad/Renderer/VertexArray.h"
#include "Salad/Renderer/Shader.h"
#include "Salad/Renderer/RenderCommand.h"

#include <glm/ext/matrix_transform.hpp>

namespace Salad {

	struct Renderer2DStorage {
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::init() {
		s_Data = new Renderer2DStorage();
		s_Data->quadVertexArray = VertexArray::create();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices));
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
		});

		s_Data->quadVertexArray->addVertexBuffer(vertexBuffer);
		unsigned int indices[6] = { 0, 1, 2, 3, 0, 2 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, 6);
		s_Data->quadVertexArray->setIndexBuffer(indexBuffer);
		s_Data->quadVertexArray->unbind();

		s_Data->whiteTexture = Texture2D::create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->whiteTexture->setData(& whiteTextureData, sizeof(uint32_t));

		s_Data->textureShader = Shader::create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::shutdown() {
		delete s_Data;
	}

	void Renderer2D::beginScene(const OrthographicCamera camera) {
		s_Data->textureShader->bind();
		s_Data->textureShader->setMat4("u_ViewProjection", camera.getViewProjectionMatrix());
	}

	void Renderer2D::endScene() {
		s_Data->textureShader->unbind();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float rotation) {
		drawQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float rotation) {

		s_Data->textureShader->setFloat4("u_Color", color);
		s_Data->whiteTexture->bind();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->quadVertexArray);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation) {
		drawTexturedQuad({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation) {

		s_Data->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		s_Data->textureShader->setFloat2("u_TexMapSize", glm::vec2(1.0f));
		s_Data->textureShader->setFloat2("u_TexMapCoords", glm::vec2(1.0f));
		texture->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data->textureShader->setMat4("u_Transform", transform);

		s_Data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->quadVertexArray);
	}

	void Renderer2D::drawSprite(const glm::vec2& position, const glm::vec2& size, const Ref<Sprite>& sprite, const float rotation) {
		drawSprite({ position.x, position.y, 0.0f }, size, sprite, rotation);
	}

	void Renderer2D::drawSprite(const glm::vec3& position, const glm::vec2& size, const Ref<Sprite>& sprite, const float rotation) {
		s_Data->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		sprite->getTextureMap()->getTexture()->bind();

		float texMapSizeH = 1.0f / (float)sprite->getTextureMap()->getGridCountH();
		float texMapSizeV = 1.0f / (float)sprite->getTextureMap()->getGridCountV();

		s_Data->textureShader->setFloat2("u_TexMapSize", glm::vec2(texMapSizeH, texMapSizeV));

		int hIndex = sprite->getSpriteIndex() % sprite->getTextureMap()->getGridCountH();
		int vIndex = sprite->getSpriteIndex() / sprite->getTextureMap()->getGridCountH();

		float hOffset = hIndex * 1.0f / sprite->getTextureMap()->getGridCountH();
		float vOffset = hIndex * 1.0f / sprite->getTextureMap()->getGridCountV();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Data->textureShader->setMat4("u_Transform", transform);
		s_Data->textureShader->setFloat2("u_TexMapCoords", glm::vec2(hIndex * hOffset, vIndex * vOffset));

		s_Data->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Data->quadVertexArray);
	}

}