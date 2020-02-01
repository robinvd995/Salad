#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/TextureMap.h"

namespace Salad {

	// TODO: Make animated sprite using an per entity animator or per sprite animator?
	class Sprite {
	
	public:
		Sprite(Ref<TextureMap>& textureMap, uint32_t spriteIndex) :
			m_TextureMap(textureMap),
			m_SpriteIndex(spriteIndex)
		{}
		~Sprite() {}

		inline uint32_t getSpriteIndex() const { return m_SpriteIndex; }
		inline Ref<TextureMap>& getTextureMap() const { return m_TextureMap; }

	private:
		Ref<TextureMap>& m_TextureMap;
		uint32_t m_SpriteIndex;
	};

}
