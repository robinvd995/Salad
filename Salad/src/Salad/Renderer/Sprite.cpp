#include "sldpch.h"
#include "Sprite.h"

#include "Salad/Renderer/TextureManager.h"
#include "Salad/Core/BinaryIO.h"

namespace Salad {

	Sprite::Sprite(std::string& spriteTexture, uint32_t texCoordX, uint32_t texCoordY, uint32_t spriteWidth, uint32_t spriteHeight, 
		float frameTime, uint32_t frameCount, uint32_t spritesPerRow) :
		m_SpriteTexture(TextureManager::get().getTexture2D(spriteTexture)),
		m_TexCoordX(texCoordX),
		m_TexCoordY(texCoordY),
		m_SpriteWidth(spriteWidth),
		m_SpriteHeight(spriteHeight),
		m_FrameTime(frameTime),
		m_FrameCount(frameCount),
		m_SpritesPerRow(spritesPerRow)
	{
		/*SLD_CORE_TRACE("Texture: {0}", m_SpriteTexture);
		SLD_CORE_TRACE("m_TexCoordX: {0}", m_TexCoordX);
		SLD_CORE_TRACE("m_TexCoordY: {0}", m_TexCoordY);
		SLD_CORE_TRACE("m_SpriteWidth: {0}", m_SpriteWidth);
		SLD_CORE_TRACE("m_SpriteHeight: {0}", m_SpriteHeight);
		SLD_CORE_TRACE("m_FrameTime: {0}", m_FrameTime);
		SLD_CORE_TRACE("m_FrameCount: {0}", m_FrameCount);
		SLD_CORE_TRACE("m_SpritesPerRow: {0}", m_SpritesPerRow);*/
	}

	void SpriteAnimator::updateAnimator(Timestep ts, float frameTime, uint32_t frameCount) {
		float newTime = m_CurrentFrameTime + ts;
		int framesPassed = (int) floor(newTime / frameTime);

		m_CurrentFrame = (m_CurrentFrame + framesPassed) % frameCount;
		m_CurrentFrameTime = newTime - ((float)framesPassed * frameTime);
	}

	void SpriteLibrary::loadSprite(std::string& filepath) {
		BinaryReader reader(filepath);

		std::string spriteId = reader.readString();
		std::string textureId = reader.readString();
		int texCoordX = reader.readInt();
		int texCoordY = reader.readInt();
		int spriteWidth = reader.readInt();
		int spriteHeight = reader.readInt();
		int frameCount = reader.readInt();
		float frameTime = reader.readFloat();
		int spritesPerRow = reader.readInt();

		Ref<Sprite> sprite = createRef<Sprite>(textureId, texCoordX, texCoordY, spriteWidth, spriteHeight, frameTime, frameCount, spritesPerRow);
		m_SpriteMap[spriteId] = sprite;
	}

	Ref<Sprite> SpriteLibrary::getSprite(const std::string& sprite) {
		SLD_CORE_ASSERT((m_SpriteMap.find(sprite) != m_SpriteMap.end()), "Trying to get sprite '" + sprite + "', but was not loaded!");
		return m_SpriteMap[sprite];
	}
}
