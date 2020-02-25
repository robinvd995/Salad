#pragma once

#include "Gui.h"

namespace Salad {

	class GuiTessellator {
	
	public:
		static void tessellateTiledElement(int x, int y, int width, int height, TiledElementStyle& style);

	};
}