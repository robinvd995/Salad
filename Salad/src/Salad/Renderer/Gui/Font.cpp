#include "sldpch.h"
#include "Font.h"

#include "Salad/Renderer/TextureManager.h"
#include "Salad/Renderer/Tessellator.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define SLD_GLYPHS_PER_ROW 16

namespace Salad {

	// -------- Font Start -------- //

	Font::Font(uint32_t fontSize) :
		m_FontSize(fontSize)
	{}

	void Font::initFont(const char* fontFile) {

		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			SLD_CORE_ERROR("Could not initialize FreeType Library");

		FT_Face face;
		if (FT_New_Face(ft, fontFile, 0, &face))
			SLD_CORE_ERROR("Failed to load font '{0}'!", fontFile);

		FT_Set_Pixel_Sizes(face, 0, m_FontSize);

		int textureSize = 128;
		while(textureSize < m_FontSize * SLD_GLYPHS_PER_ROW) {
			textureSize *= 2;
		}

		m_TextureSize = textureSize;

		initializeImage(textureSize, textureSize);

		unsigned int offset = 0;

		for(unsigned int i = offset; i < SLD_GLYPHS_TO_LOAD + offset; i++) {
			unsigned char c = (unsigned char)i;

			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				SLD_CORE_ERROR("Failed to load glyph!");
		
			uint32_t glyphWidth = face->glyph->bitmap.width;
			uint32_t glyphHeight = face->glyph->bitmap.rows;
			uint32_t size = glyphWidth * glyphHeight;

			int index = i - offset;
			int posX = (index % SLD_GLYPHS_PER_ROW) * m_FontSize;
			int posY = (index / SLD_GLYPHS_PER_ROW) * m_FontSize;

			std::vector<unsigned char> dataArray;
			if (size != 0) {
				dataArray.reserve((size_t)size * 4);

				for (int i = 0; i < size; i++) {
					dataArray.push_back(255);
					dataArray.push_back(255);
					dataArray.push_back(255);
					dataArray.push_back(face->glyph->bitmap.buffer[i]);
				}

				setImageData(posX, posY, glyphWidth, glyphHeight, &dataArray[0]);
			}

			m_GlyphArray[c].u = posX;
			m_GlyphArray[c].v = posY;
			m_GlyphArray[c].width = glyphWidth;
			m_GlyphArray[c].height = glyphHeight;
			m_GlyphArray[c].bearingX = face->glyph->bitmap_left;
			m_GlyphArray[c].bearingY = face->glyph->bitmap_top;
			m_GlyphArray[c].advance = face->glyph->advance.x >> 6;
		}
	}

	Ref<VertexArray> Font::generateText(std::string text) {
		
		Tessellator::start(TessellatorMode::TRIANGLES);
		Tessellator::setTextureResolution(1.0f / m_TextureSize);

		int xPos = 0;

		std::string::const_iterator it;
		for(it = text.begin(); it != text.end(); it++) {
			FontGlyph glyph = m_GlyphArray[*it];

			Tessellator::pushQuad(xPos + glyph.bearingX, -glyph.bearingY, 0, glyph.width, glyph.height, glyph.u, glyph.v, glyph.width, glyph.height);
			xPos += glyph.advance;
		}

		std::vector<float> vertexData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> indexData = Tessellator::getIndexBuffer();
		Tessellator::end();

		Ref<VertexArray> renderId = VertexArray::create();

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::create(&vertexData[0], vertexData.size() * sizeof(float), SLD_DYNAMIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
		});

		renderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&indexData[0], indexData.size());
		renderId->setIndexBuffer(indexBuffer);
		renderId->unbind();

		return renderId;
	}

	// -------- Font End -------- //
}
