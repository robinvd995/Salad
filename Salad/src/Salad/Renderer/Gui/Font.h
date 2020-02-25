#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/Texture.h"
#include "Salad/Renderer/VertexArray.h"

#include <vector>

#define SLD_GLYPHS_TO_LOAD 256

namespace Salad {

	struct FontGlyph {
		unsigned int u = 0, v = 0;
		unsigned int width = 0, height = 0;
		int bearingX = 0, bearingY = 0;
		unsigned int advance = 0;

		FontGlyph() {}
	};
	
	class Font {
		
	public:

		Font(uint32_t fontSize);

		void initFont(const char* fontFile);

		inline Ref<Texture2D> getFontTexture() const { return m_FontTexture; }

		virtual void initializeImage(uint32_t imageWidth, uint32_t imageHeight) = 0;
		virtual void setImageData(uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) = 0;

		Ref<VertexArray> generateText(std::string text);

	protected:
		uint32_t m_FontSize;
		uint32_t m_TextureSize = 128;
		FontGlyph m_GlyphArray[SLD_GLYPHS_TO_LOAD];
		Ref<Texture2D> m_FontTexture;
	};
}
