#pragma once

#include "Salad/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Salad {

	class OpenGLVertexBuffer : public VertexBuffer {

	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size, uint32_t drawMode = GL_STATIC_DRAW);
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

	class OpenGLUniformBuffer : public UniformBuffer {
	
	public:
		OpenGLUniformBuffer(const uint32_t binding, const uint32_t size);

		virtual void updateBuffer(const void* data) override;
		virtual void updateBuffer(const uint32_t offset, const uint32_t size, const void* data) override;

	private:
		uint32_t m_BufferId = 0;

	};
}