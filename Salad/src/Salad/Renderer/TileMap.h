#pragma once

#include "Salad/Renderer/VertexArray.h"

namespace Salad {

	class TileMap {
	
	public:

		TileMap(int width, int height, int gridCount, float tileWidth = 1.0f, float tileHeight = 1.0f);
		~TileMap();

		void setTileData(int index, int tile);
		void buildTileMap();

		inline Ref<VertexArray> getRenderId() const { return m_RenderId; }

	private:
		int m_Width, m_Height;
		int m_GridCount;
		float m_TileWidth, m_TileHeight;
		short* m_TileData;

		Ref<VertexArray> m_RenderId;
	};

}
