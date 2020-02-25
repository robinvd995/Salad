#include "sldpch.h"
#include "OpenGLFont.h"

namespace Salad {

	void OpenGLFont::initializeImage(uint32_t imageWidth, uint32_t imageHeight) {
		uint32_t textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

		m_FontTexture = createRef<OpenGLTexture2D>(textureId, imageWidth, imageHeight, GL_RED, GL_RED);
	}

	void OpenGLFont::setImageData(uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) {
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, y, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}
