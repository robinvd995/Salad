#pragma once

#include "Salad/Renderer/VertexArray.h"

#include <vector>

namespace Salad {

	class OpenGLVertexArray : public VertexArray {

	public:
		OpenGLVertexArray();
		~OpenGLVertexArray() {}

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t addVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) override;
		virtual void setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;

		virtual void updateVertexBuffer(uint32_t index, uint32_t offset, uint32_t size, void* data) override;

		virtual Ref<IndexBuffer> getIndexBuffer() const override { return m_IndexBuffer; }

	private:
		std::vector<Ref<VertexBuffer>> m_VertexBuffers;
		Ref<IndexBuffer> m_IndexBuffer;
		uint32_t m_VertexArrayId;
		uint32_t m_CurVBIndex = 0;
	};

}