#pragma once

#include "Salad/Renderer/Buffer.h"

namespace Salad {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual void setLayout(const BufferLayout layout) override { m_Layout = layout; };
		virtual const BufferLayout& getLayout() override { return m_Layout; }

	private:
		uint32_t m_BufferId;
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer {

	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual uint32_t getCount() const override { return m_Count; }

	private:
		uint32_t m_BufferId;
		uint32_t m_Count;
	};
}