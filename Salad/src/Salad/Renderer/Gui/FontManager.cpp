#include "sldpch.h"
#include "FontManager.h"

#include "Platform/OpenGL/OpenGLFont.h"

namespace Salad {

	// -------- FontManager Start -------- //

	Ref<Font> FontManager::loadFontImpl(const char* font, uint32_t fontSize) {
		Ref<Font> fontObj = createRef<OpenGLFont>(fontSize);
		fontObj->initFont(font);
		return fontObj;
	}

	// -------- FontManager End -------- //
}
