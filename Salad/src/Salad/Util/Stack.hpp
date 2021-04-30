#pragma once

namespace Salad::Util {

	template<typename T>
	class Stack {

	public:
		Stack() = default;
		Stack(const Stack&) = delete;
		~Stack() = default;

		void alloc(size_t size) {
			m_Size = size;
			m_Data = (T*)malloc(sizeof(T) * size);
		}

		void release() {
			if(m_Data) free(m_Data);
			m_Data = nullptr;
		}

		void push(const T& data) {
			m_Data[m_StackPointer >= m_Size ? 0 : m_StackPointer] = data; // TODO: replace % with ASSERT or another operation
			m_StackPointer++;
		}

		T& peek() {
			return m_Data[m_StackPointer < 0 ? 0 : m_StackPointer-1]; // TODO: replace % with ASSERT or another operation
		}

		T& pop() {
			return m_Data[m_StackPointer < 0 ? 0 : --m_StackPointer]; // TODO: replace % with ASSERT or another operation
		}

	private:
		uint64_t m_StackPointer = 0;
		size_t m_Size;
		T* m_Data;
	};

}