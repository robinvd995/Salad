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

	/*class OpenGLFramebuffer : public Framebuffer {

	public:
		OpenGLFramebuffer(uint32_t width, uint32_t height);
		virtual ~OpenGLFramebuffer() override;

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual Ref<Texture2D> getColorBuffer() const override { return m_ColorBuffer; }

	private:
		uint32_t m_Width, m_Height;

		uint32_t m_FramebufferId, m_RenderBufferId;
		Ref<Texture2D> m_ColorBuffer;
	};*/
}