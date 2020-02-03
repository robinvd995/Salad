#pragma once

#include "Salad/Renderer/Texture.h"
#include "Salad/Entity/Entity.h"
#include "Salad/Renderer/OrthographicCamera.h"

#include <glm/glm.hpp>

namespace Salad {

	class Renderer2D {

	public:
		static void init();
		static void shutdown();

		static void beginScene(const OrthographicCamera camera);
		static void endScene();

		static void drawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color, const float rotation = 0.0f);
		static void drawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, const float rotation = 0.0f);

		static void drawTexturedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f);
		static void drawTexturedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const float rotation = 0.0f);

		static void drawSprite(const glm::vec2& position, const glm::vec2& size, const SpriteRenderInformation& sri);
		static void drawSprite(const glm::vec3& position, const glm::vec2& size, const SpriteRenderInformation& sri);
	};
}