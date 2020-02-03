#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/Texture.h"

namespace Salad {

	class Sprite {

	public:
		Sprite() {}
		Sprite(const Sprite& other) = delete;
		~Sprite() {}

		inline uint32_t getTexCoordX() const { return m_TexCoordX; }
		inline uint32_t getTexCoordY() const { return m_TexCoordY; }
		inline float getFrameTime() const { return m_FrameTime; }
		inline uint32_t getFrameCount() const { return m_FrameCount; }
		inline uint32_t getSpriteWidth() const { return m_SpriteWidth; }
		inline uint32_t getSpriteHeight() const { return m_SpriteHeight; }
		inline uint32_t getSpritesPerRow() const { return m_SpritesPerRow; }

	private:
		Ref<Texture2D> m_SpriteTexture;

		uint32_t m_TexCoordX = 0, m_TexCoordY = 0;
		float m_FrameTime = 1.0f;
		uint32_t m_FrameCount = 1;
		uint32_t m_SpriteWidth = 16, m_SpriteHeight = 16, m_SpritesPerRow = 1;
	};

	class SpriteLibrary {

	public:

		static SpriteLibrary& get(){
			static SpriteLibrary instance;
			return instance;
		}

	private:
		SpriteLibrary() {}
		~SpriteLibrary() {}
	};
}
