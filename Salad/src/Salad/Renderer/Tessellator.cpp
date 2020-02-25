#include "sldpch.h"
#include "Tessellator.h"

namespace Salad {

	void Tessellator::startImpl(TessellatorMode mode) {
		m_Mode = mode;
		SLD_CORE_ASSERT(m_VertexBuffer.size() == 0, "Tessellator vertex buffer is not empty while attempting to start!");
		SLD_CORE_ASSERT(m_IndexBuffer.size() == 0, "Tessellator index buffer is not empty while attempting to start!");
		m_Index = 0;
	}

	void Tessellator::endImpl() {
		m_Mode = TessellatorMode::NONE;
		m_VertexBuffer.clear();
		m_IndexBuffer.clear();
		m_Index = 0;
		m_TextureResolution = 1.0f;
	}

	void Tessellator::pushVertexImpl(float x, float y, float z, float u, float v) {
		m_VertexBuffer.push_back(x);
		m_VertexBuffer.push_back(y);
		m_VertexBuffer.push_back(z);
		m_VertexBuffer.push_back(u * m_TextureResolution);
		m_VertexBuffer.push_back(v * m_TextureResolution);
	}

	void Tessellator::pushQuadImpl(float x, float y, float z, float width, float height, float u, float v, float texWidth, float texHeight) {
		pushVertexImpl(x, y, z, u, v);
		pushVertexImpl(x + width, y, z, u + texWidth, v);
		pushVertexImpl(x, y + height, z, u, v + texHeight);
		pushVertexImpl(x + width, y + height, z, u + texWidth, v + texHeight);
		makeQuadImpl();
	}

	void Tessellator::makeTriangleImpl() {
		switch(m_Mode){
			case TessellatorMode::NONE: 
			case TessellatorMode::QUADS:
				SLD_CORE_ASSERT_FAIL("Trying to create a triangle using a unsupported draw mode!");
				break;

			case TessellatorMode::TRIANGLES:
				m_IndexBuffer.push_back(m_Index + 0);
				m_IndexBuffer.push_back(m_Index + 0);
				m_IndexBuffer.push_back(m_Index + 0);
				m_Index += 3;
				break;
		}
	}

	void Tessellator::makeQuadImpl() {
		switch (m_Mode) {
			case TessellatorMode::NONE:
				SLD_CORE_ASSERT_FAIL("Trying to create a quad using a unsupported draw mode!");
				break;

			case TessellatorMode::QUADS:
				m_IndexBuffer.push_back(m_Index + 0);
				m_IndexBuffer.push_back(m_Index + 1);
				m_IndexBuffer.push_back(m_Index + 3);
				m_IndexBuffer.push_back(m_Index + 2);
				break;

			case TessellatorMode::TRIANGLES:
				m_IndexBuffer.push_back(m_Index + 0);
				m_IndexBuffer.push_back(m_Index + 1);
				m_IndexBuffer.push_back(m_Index + 3);
				m_IndexBuffer.push_back(m_Index + 0);
				m_IndexBuffer.push_back(m_Index + 3);
				m_IndexBuffer.push_back(m_Index + 2);
				m_Index += 4;
				break;
		}
	}
}