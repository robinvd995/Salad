#pragma once

#include "OpenGLTexture.h"

#include "Salad/Renderer/Gui/Font.h"

#include <glad/glad.h>

namespace Salad {

	class OpenGLFont : public Font {
	
	public:
		OpenGLFont(uint32_t fontSize) : Font(fontSize) {}

		virtual void initializeImage(uint32_t imageWidth, uint32_t imageHeight) override;
		virtual void setImageData(uint32_t x, uint32_t y, uint32_t width, uint32_t height, void* data) override;
	};

}
