#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Core/Timestep.h"
#include "Salad/Renderer/Texture.h"

namespace Salad {

	class SpriteLibrary;

	class Sprite {

		friend class SpriteLibrary;

	public:
		Sprite() = delete;
		Sprite(const Sprite& other) = delete;

		Sprite(std::string& spriteTexture, uint32_t texCoordX, uint32_t texCoordY, uint32_t spriteWidth, uint32_t spriteHeight,
			float frameTime, uint32_t frameCount, uint32_t spritesPerRow);
		~Sprite() {}

		// Calculate the actuall render values so the renderer doesnt have to every frame
		// Can still let the renderer calculate the location of the animation of the given sprite, might heap alloc all values later


		inline Ref<Texture2D> getSpriteTexture() const { return m_SpriteTexture; }
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
		uint32_t m_SpriteWidth = 16, m_SpriteHeight = 16;
		float m_FrameTime = 1.0f;
		uint32_t m_FrameCount = 1, m_SpritesPerRow = 1;
	};

	class SpriteAnimator {

	public:
		void updateAnimator(Timestep ts, float frameTime, uint32_t frameCount);

		inline uint32_t getCurrentFrame() const { return m_CurrentFrame; }
		inline float getCurrentFrameTime() const { return m_CurrentFrameTime; }
	private:
		uint32_t m_CurrentFrame = 0;
		float m_CurrentFrameTime = 0;
	};

	class SpriteLibrary {

	public:

		static SpriteLibrary& get(){
			static SpriteLibrary instance;
			return instance;
		}

		void loadSprite(std::string& filepath);
		Ref<Sprite> getSprite(const std::string& sprite);

	private:
		SpriteLibrary() {}
		~SpriteLibrary() {}
	private:
		std::unordered_map<std::string, Ref<Sprite>> m_SpriteMap;
	};
}
