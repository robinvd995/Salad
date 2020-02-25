#include "sldpch.h"
#include "GuiTessellator.h"

#include "Salad/Renderer/Tessellator.h"

namespace Salad {

	void GuiTessellator::tessellateTiledElement(int x, int y, int width, int height, TiledElementStyle& style) {

		int horizontalLength = style.width - style.leftWidth - style.rightWidth;
		int verticalLength = style.height - style.topHeight - style.bottomHeight;

		Tessellator::pushQuad(0, 0, 0, style.leftWidth, style.topHeight, style.posX, style.posY, style.leftWidth, style.topHeight);
		Tessellator::pushQuad(width - style.rightWidth, 0, 0, style.rightWidth, style.topHeight, style.posX + style.width - style.rightWidth, style.posY, style.rightWidth, style.topHeight);
		Tessellator::pushQuad(0, height - style.bottomHeight, 0, style.leftWidth, style.bottomHeight, style.posX, style.posY + style.height - style.bottomHeight, style.leftWidth, style.bottomHeight);
		Tessellator::pushQuad(width - style.rightWidth, height - style.bottomHeight, 0, style.rightWidth, style.bottomHeight, style.posX + style.width - style.rightWidth, style.posY + style.height - style.bottomHeight, style.rightWidth, style.bottomHeight);

		int posX = style.leftWidth;
		int posY = 0;
		while(posX < width - style.rightWidth) {
			int toDraw = std::min((int)((width - style.rightWidth) - posX), horizontalLength);
			Tessellator::pushQuad(posX, posY, 0, toDraw, style.topHeight, style.posX + style.leftWidth, style.posY, toDraw, style.topHeight);
			posX += horizontalLength;
		}

		posX = style.leftWidth;
		posY = height - style.bottomHeight;
		while(posX < width - style.rightWidth) {
			int toDraw = std::min((int)((width - style.rightWidth) - posX), horizontalLength);
			Tessellator::pushQuad(posX, posY, 0, toDraw, style.bottomHeight, style.posX + style.leftWidth, style.posY + style.height - style.bottomHeight, toDraw, style.bottomHeight);
			posX += horizontalLength;
		}

		posX = 0;
		posY = style.topHeight;
		while(posY < height - style.bottomHeight) {
			int toDraw = std::min((int)((height - style.bottomHeight) - posY), verticalLength);
			Tessellator::pushQuad(posX, posY, 0, style.leftWidth, toDraw, style.posX, style.posY + style.topHeight, style.leftWidth, toDraw);
			posY += toDraw;
		}

		posX = width - style.rightWidth;
		posY = style.topHeight;
		while (posY < height - style.bottomHeight) {
			int toDraw = std::min((int)((height - style.bottomHeight) - posY), verticalLength);
			Tessellator::pushQuad(posX, posY, 0, style.rightWidth, toDraw, style.posX + style.width - style.rightWidth, style.posY + style.topHeight, style.rightWidth, toDraw);
			posY += toDraw;
		}

		posX = style.leftWidth;
		posY = style.topHeight;
		while(posX < width - style.rightWidth) {
			int toDrawX = std::min((int)((width - style.rightWidth) - posX), horizontalLength);
			while(posY < height - style.bottomHeight) {
				int toDrawY = std::min((int)((height - style.bottomHeight) - posY), verticalLength);
				Tessellator::pushQuad(posX, posY, 0, toDrawX, toDrawY, style.posX + style.leftWidth, style.posY + style.topHeight, toDrawX, toDrawY);
				posY += verticalLength;
			}
			posX += horizontalLength;
			posY = style.topHeight;
		}
	}

}