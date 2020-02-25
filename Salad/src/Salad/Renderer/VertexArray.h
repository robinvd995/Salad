#pragma once
#include <cstdint>
#include "Buffer.h"
#include <memory>

namespace Salad {

	class VertexArray {
	public:
		virtual ~VertexArray() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t addVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) = 0;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;

		virtual void updateVertexBuffer(uint32_t index, uint32_t offset, uint32_t size, void* data) = 0;

		virtual Ref<IndexBuffer> getIndexBuffer() const = 0;

		static Ref<VertexArray> create();
	};
}