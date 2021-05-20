#pragma once

#include <cstdint>
#include <memory>
#include <iostream>

namespace Salad::Util {

	class ByteBuffer {

	public:
		ByteBuffer() = default;
		ByteBuffer(char* data, size_t size) : m_BufferSize(size), m_IsDataOwner(false){ m_BufferData = data; m_Iterator = 0; }
		ByteBuffer(size_t maxSegmentedWriteSize) : m_MaxSegmentedWriteSize(maxSegmentedWriteSize) {}
		~ByteBuffer() = default;

		template<typename T>
		void allocate(size_t count) {
			if (m_IsDataOwner) {
				m_BufferSize = sizeof(T) * count;
				m_BufferData = (char*)malloc(m_BufferSize);
			}
		}

		template<typename T>
		size_t getBufferElementCount() { return m_BufferSize / sizeof(T); }
		size_t getBufferSize() { return m_BufferSize; }

		template<typename T>
		void write(T value) {
			memcpy(&m_BufferData[m_Iterator], (void*)(&value), sizeof(T));
			m_Iterator += sizeof(T);
		}

		template<typename T>
		T read() {
			size_t pos = m_Iterator;
			m_Iterator += sizeof(T);
			return *((T*)&m_BufferData[(pos)]);
		}

		template<typename T>
		void writeArray(T* data, size_t count) {
			size_t size = count * sizeof(T);
			size_t it = 0;
			size_t remainingSize = size;
			while (remainingSize > 0) {
				size_t sizeToWrite = remainingSize;
				if (sizeToWrite > m_MaxSegmentedWriteSize) sizeToWrite = m_MaxSegmentedWriteSize;

				memcpy((void*)&m_BufferData[m_Iterator], (void*)&data[it], sizeToWrite);
				m_Iterator += sizeToWrite;
				it += sizeToWrite / sizeof(T);

				remainingSize -= sizeToWrite;
			}
		}

		template<typename T>
		T* readArray(size_t count) {
			size_t size = count * sizeof(T);
			size_t pos = m_Iterator;
			m_Iterator += size;
			return ((T*)&m_BufferData[pos]);
		}

		size_t getIteratorIndex() { return m_Iterator; }

		void moveIterator(size_t newpos) { m_Iterator = newpos; }

		void mark() { m_MarkedPos = m_Iterator; }
		void moveToMark() { m_Iterator = m_MarkedPos; }

		template<typename T>
		void skip(size_t amount) { m_Iterator += (sizeof(T) * amount); }

		template<typename T>
		T* get() { return (T*)m_BufferData; }

		void freeBuffer() { if(m_IsDataOwner) free((char*)m_BufferData); }

	public:
		friend std::ostream& operator<<(std::ostream& os, const ByteBuffer& buffer) {
			os.write(buffer.m_BufferData, buffer.m_BufferSize);
			return os;
		}

	private:
		char* m_BufferData = nullptr;
		size_t m_BufferSize = 0;
		size_t m_Iterator = 0;

		size_t m_MarkedPos = 0;
		size_t m_MaxSegmentedWriteSize = 65536;

		bool m_IsDataOwner = true;
	};

	class FloatBuffer {

	public:
		FloatBuffer() = default;
		~FloatBuffer() = default;

		void allocate(size_t count) { m_BufferBase.allocate<float>(count); }
		void freeBuffer() { m_BufferBase.freeBuffer(); }

		void write(float value) { m_BufferBase.write<float>(value); }
		float read() { m_BufferBase.read<float>(); }

		void writeArray(float* data, size_t count) { m_BufferBase.writeArray<float>(data, count); }
		float* readArray(size_t count) { return m_BufferBase.readArray<float>(count); }

		void moveIterator(size_t newpos) { m_BufferBase.moveIterator(newpos * sizeof(float)); }
		void mark() { m_BufferBase.mark(); }
		void moveToMark() { m_BufferBase.moveToMark(); }

		float* get() { return m_BufferBase.get<float>(); }
		size_t getBufferSize() { return m_BufferBase.getBufferElementCount<float>(); }

	private:
		ByteBuffer m_BufferBase;
	};

	class Int32Buffer {
	public:
		Int32Buffer() = default;
		~Int32Buffer() = default;

		void allocate(size_t count) { m_BufferBase.allocate<int32_t>(count); }
		void freeBuffer() { m_BufferBase.freeBuffer(); }

		void write(int32_t value) { m_BufferBase.write<int32_t>(value); }
		int32_t read() { m_BufferBase.read<int32_t>(); }

		void writeArray(int32_t* data, size_t count) { m_BufferBase.writeArray<int32_t>(data, count); }
		int32_t* readArray(size_t count) { return m_BufferBase.readArray<int32_t>(count); }

		void moveIterator(size_t newpos) { m_BufferBase.moveIterator(newpos * sizeof(int32_t)); }
		void mark() { m_BufferBase.mark(); }
		void moveToMark() { m_BufferBase.moveToMark(); }

		int32_t* get() { return m_BufferBase.get<int32_t>(); }
		size_t getBufferSize() { return m_BufferBase.getBufferElementCount<int32_t>(); }

	private:
		ByteBuffer m_BufferBase;
	};

	class UInt32Buffer {
	public:
		UInt32Buffer() = default;
		~UInt32Buffer() = default;

		void allocate(size_t count) { m_BufferBase.allocate<uint32_t>(count); }
		void freeBuffer() { m_BufferBase.freeBuffer(); }

		void write(uint32_t value) { m_BufferBase.write<uint32_t>(value); }
		uint32_t read() { m_BufferBase.read<uint32_t>(); }

		void writeArray(uint32_t* data, size_t count) { m_BufferBase.writeArray<uint32_t>(data, count); }
		uint32_t* readArray(size_t count) { return m_BufferBase.readArray<uint32_t>(count); }

		void moveIterator(size_t newpos) { m_BufferBase.moveIterator(newpos * sizeof(uint32_t)); }
		void mark() { m_BufferBase.mark(); }
		void moveToMark() { m_BufferBase.moveToMark(); }

		uint32_t* get() { return m_BufferBase.get<uint32_t>(); }
		size_t getBufferSize() { return m_BufferBase.getBufferElementCount<uint32_t>(); }

	private:
		ByteBuffer m_BufferBase;
	};
}
