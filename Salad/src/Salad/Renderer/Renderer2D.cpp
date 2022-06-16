#include "sldpch.h"
#include "Renderer2D.h"

#include "Salad/Renderer/VertexArray.h"
#include "Salad/Renderer/Shader.h"
#include "Salad/Renderer/RenderCommand.h"
#include "Salad/Renderer/TextureManager.h"

#include <glm/ext/matrix_transform.hpp>

namespace Salad {

	struct Renderer2DStorage {
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> textureShader;
		Ref<Texture2D> whiteTexture;
	};

	static Renderer2DStorage* s_Render2DData;

	void Renderer2D::init() {
		s_Render2DData = new Renderer2DStorage();
		s_Render2DData->quadVertexArray = VertexArray::create();

		float vertices[4 * 5] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(vertices, sizeof(vertices), SLD_STATIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
		});

		s_Render2DData->quadVertexArray->addVertexBuffer(vertexBuffer);
		unsigned int indices[6] = { 0, 1, 2, 3, 0, 2 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indices, 6);
		s_Render2DData->quadVertexArray->setIndexBuffer(indexBuffer);
		s_Render2DData->quadVertexArray->unbind();

		s_Render2DData->whiteTexture = TextureManager::get().loadTexture2D("default_white", 1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Render2DData->whiteTexture->setData(&whiteTextureData, sizeof(uint32_t));

		s_Render2DData->textureShader = Shader::create("assets/shaders/Texture.glsl");
	}

	void Renderer2D::shutdown() {
		delete s_Render2DData;
	}

	void Renderer2D::beginScene(const Camera& camera, const glm::mat4& transform) {
		s_Render2DData->textureShader->bind();
		glm::mat4 viewProjMatrix = camera.getProjection() * glm::inverse(transform);
		s_Render2DData->textureShader->setMat4("u_ViewProjection", viewProjMatrix);
	}

	void Renderer2D::endScene() {
		s_Render2DData->textureShader->unbind();
	}

	void Renderer2D::drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float rotation) {
		drawQuad({ position.x, position.y, 0.0f }, size, color, rotation);
	}

	void Renderer2D::drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float rotation) {

		s_Render2DData->textureShader->setFloat4("u_Color", color);
		s_Render2DData->whiteTexture->bind();

		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), position) * 
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Render2DData->textureShader->setMat4("u_Transform", transform);

		s_Render2DData->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Render2DData->quadVertexArray);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation) {
		drawTexturedQuad({ position.x, position.y, 0.0f }, size, texture, rotation);
	}

	void Renderer2D::drawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation) {

		s_Render2DData->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVSize", glm::vec2(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVCoords", glm::vec2(1.0f));
		texture->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Render2DData->textureShader->setMat4("u_Transform", transform);

		s_Render2DData->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Render2DData->quadVertexArray);
	}

	/*void Renderer2D::drawSprite(const glm::vec2& position, const glm::vec2& size, const SpriteRenderInformation& sri) {
		drawSprite({ position.x, position.y, 0.0f }, size, sri);
	}

	void Renderer2D::drawSprite(const glm::vec3& position, const glm::vec2& size, const SpriteRenderInformation& sri) {
		s_Render2DData->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		sri.spriteTexture->bind();

		s_Render2DData->textureShader->setFloat2("u_UVSize", glm::vec2(sri.sizeU, sri.sizeV));
		s_Render2DData->textureShader->setFloat2("u_UVCoords", glm::vec2(sri.posU, sri.posV));

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Render2DData->textureShader->setMat4("u_Transform", transform);

		s_Render2DData->quadVertexArray->bind();
		RenderCommand::drawIndexed(s_Render2DData->quadVertexArray);
	}*/

	void Renderer2D::drawTileMap(const glm::vec2& position, const glm::vec2& size, const Ref<TileMap> tilemap) {
		drawTileMap({ position.x, position.y, 0.0f }, size, tilemap);
	}

	void Renderer2D::drawTileMap(const glm::vec3& position, const glm::vec2& size, const Ref<TileMap> tilemap) {

		s_Render2DData->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVSize", glm::vec2(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVCoords", glm::vec2(1.0f));

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			//glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Render2DData->textureShader->setMat4("u_Transform", transform);

		tilemap->getRenderId()->bind();
		RenderCommand::drawIndexed(tilemap->getRenderId());
	}

	void Renderer2D::drawVertexArray(const glm::vec2& position, const glm::vec2& size, const Ref<VertexArray> vertexArray) {
		drawVertexArray({ position.x, position.y, 0.0f }, size, vertexArray);
	}

	void Renderer2D::drawVertexArray(const glm::vec3& position, const glm::vec2& size, const Ref<VertexArray> vertexArray) {
		s_Render2DData->textureShader->setFloat4("u_Color", glm::vec4(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVSize", glm::vec2(1.0f));
		s_Render2DData->textureShader->setFloat2("u_UVCoords", glm::vec2(0.0f));

		//s_Render2DData->whiteTexture->bind();

		glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position) *
			glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

		s_Render2DData->textureShader->setMat4("u_Transform", transform);
		vertexArray->bind();
		RenderCommand::drawIndexed(vertexArray);
	}
}