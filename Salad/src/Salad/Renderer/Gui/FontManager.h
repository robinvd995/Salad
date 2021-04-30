#pragma once

#include "Salad/Renderer/Gui/Font.h"

namespace Salad {

	class FontManager {

	public:

		static Ref<Font> loadFont(const char* font, uint32_t fontSize) { return get().loadFontImpl(font, fontSize); }

	private:

		static FontManager& get() {
			static FontManager instance;
			return instance;
		}

	private:
		FontManager() {}
		~FontManager() {}

		Ref<Font> loadFontImpl(const char* font, uint32_t fontSize);
	};

}
