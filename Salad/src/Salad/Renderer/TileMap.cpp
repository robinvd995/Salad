#include "sldpch.h"
#include "TileMap.h"

#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/RenderCommand.h"

#include <cstdlib>


namespace Salad {

	TileMap::TileMap(int width, int height, int gridCount, float tileWidth, float tileHeight) :
		m_Width(width),
		m_Height(height),
		m_GridCount(gridCount),
		m_TileWidth(tileWidth),
		m_TileHeight(tileHeight),
		m_TileData(new short[width * height])
	{
		for(int i = 0; i < m_Width * m_Height; i++) {
			m_TileData[i] = rand() % 16;
		}
	}

	TileMap::~TileMap() {
		delete[] m_TileData;
	}

	void TileMap::setTileData(int index, int tile) {
		m_TileData[index] = tile;
	}

	void TileMap::buildTileMap() {

		float resolution = 1.0f / m_GridCount;

		Tessellator::start(TessellatorMode::TRIANGLES);
		for (int y = 0; y < m_Height; y++) {
			for (int x = 0; x < m_Width; x++) {

				int tileIndex = y * m_Width + x;
				int tileId = m_TileData[tileIndex];
				if (tileId == -1) continue;

				float tilePosX = x * m_TileWidth;
				float tilePosY = y * m_TileHeight;
				float texCoordX = (tileId % m_GridCount) * resolution;
				float texCoordY = (tileId / m_GridCount) * resolution;

				Tessellator::pushVertex(tilePosX, tilePosY, 0.0f, texCoordX, texCoordY);
				Tessellator::pushVertex(tilePosX + m_TileWidth, tilePosY, 0.0f, texCoordX + resolution, texCoordY);
				Tessellator::pushVertex(tilePosX, tilePosY + m_TileHeight, 0.0f, texCoordX, texCoordY + resolution);
				Tessellator::pushVertex(tilePosX + m_TileWidth, tilePosY + m_TileHeight, 0.0f, texCoordX + resolution, texCoordY + resolution);
				Tessellator::makeQuad();
			}
		}

		std::vector<float> tileMapRenderData = Tessellator::getVertexBuffer();
		std::vector<uint32_t> tileMapIndices = Tessellator::getIndexBuffer();
		Tessellator::end();

		SLD_CORE_ASSERT((tileMapRenderData.size() > 0 && tileMapIndices.size() > 0), "Loading tilemap but no tiles have been registered on the map!");
		m_RenderId = VertexArray::create();

		Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(&tileMapRenderData[0], tileMapRenderData.size() * sizeof(float), SLD_STATIC_DRAW);
		vertexBuffer->setLayout({
			{"a_Position", Salad::ShaderDataType::Float3},
			{"a_TexCoord", Salad::ShaderDataType::Float2}
		});

		m_RenderId->addVertexBuffer(vertexBuffer);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&tileMapIndices[0], tileMapIndices.size());
		m_RenderId->setIndexBuffer(indexBuffer);
		m_RenderId->unbind();
	}
}