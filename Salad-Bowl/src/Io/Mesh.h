#pragma once

#include <vector>

namespace Salad {

	class FloatBuffer {
	
	public:
		FloatBuffer() = default;
		virtual ~FloatBuffer() = default;

		void resize(int size) { m_BufferData.resize(size); }

		void pushFloat(float f) { 
			m_BufferData[m_Iterator++] = f;
		}

		void pushFloat2(float f0, float f1) { 
			m_BufferData[m_Iterator++] = f0;
			m_BufferData[m_Iterator++] = f1;
		}

		void pushFloat3(float f0, float f1, float f2) { 
			m_BufferData[m_Iterator++] = f0;
			m_BufferData[m_Iterator++] = f1;
			m_BufferData[m_Iterator++] = f2;
		}

		void pushFloat4(float f0, float f1, float f2, float f3) { 
			m_BufferData[m_Iterator++] = f0;
			m_BufferData[m_Iterator++] = f1;
			m_BufferData[m_Iterator++] = f2;
			m_BufferData[m_Iterator++] = f3;
		}

		float* getBuffer() { return &m_BufferData.front(); }
		uint32_t getBufferSize() { return m_BufferData.size(); }

	private:
		std::vector<float> m_BufferData;
		uint32_t m_Iterator = 0;
	};

	class UIntBuffer {
	
	public:
		UIntBuffer() = default;
		virtual ~UIntBuffer() = default;

		void resize(uint32_t size) { m_BufferData.resize(size); m_CurBufferSize = size; }
		void increaseBufferSize(uint32_t size) { m_CurBufferSize += size; m_BufferData.resize(size); }

		void pushInt(uint32_t i) { m_BufferData[m_Iterator++] = i; }
		void pushInt2(uint32_t i0, uint32_t i1) { m_BufferData[m_Iterator++] = i0; m_BufferData[m_Iterator++] = i1;}
		void pushInt3(uint32_t i0, uint32_t i1, uint32_t i2) { m_BufferData[m_Iterator++] = i0;  m_BufferData[m_Iterator++] = i1;  m_BufferData[m_Iterator++] = i2; }
		void pushInt4(uint32_t i0, uint32_t i1, uint32_t i2, uint32_t i3) { m_BufferData[m_Iterator++] = i0;  m_BufferData[m_Iterator++] = i1;  m_BufferData[m_Iterator++] = i2; m_BufferData[m_Iterator++] = i3; }


		uint32_t* getBuffer() { return &m_BufferData.front(); }
		uint32_t getBufferSize() { return m_BufferData.size(); }

	private:
		std::vector<uint32_t> m_BufferData;
		uint32_t m_Iterator = 0;
		uint32_t m_CurBufferSize = 0;

	};

	class Mesh {

	public:
		Mesh() = default;
		virtual ~Mesh() = default;

		//void reserve(int size) { m_MeshData.em(); }

		inline FloatBuffer& getBuffer() { return m_MeshData; }
		inline std::vector<uint32_t>& getIndexBuffer() { return m_IndexData; }

	private:
		FloatBuffer m_MeshData;
		std::vector<uint32_t> m_IndexData;
	};

}