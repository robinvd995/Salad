#pragma once

#include "Salad/Core/Core.h"

#include "Texture.h"

namespace Salad {

	class TextureMap {

		friend class TextureManager;

	public:

		TextureMap(const TextureMap& map) = delete;
		TextureMap(Ref<Texture2D> texture, uint32_t gridCountH, uint32_t gridCountV) :
			m_Texture(texture),
			m_GridCountH(gridCountH),
			m_GridCountV(gridCountV) {
		}

		~TextureMap() {}

		inline uint32_t getGridCountH() const { return m_GridCountH; }
		inline uint32_t getGridCountV() const { return m_GridCountV; }

		inline Ref<Texture2D>& getTexture() { return m_Texture; }

	private:
		Ref<Texture2D> m_Texture;
		unsigned int m_GridCountH, m_GridCountV;
	};
}
