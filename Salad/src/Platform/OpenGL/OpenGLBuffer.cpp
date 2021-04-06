#include "sldpch.h"

#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Salad {

	// -------------VertexBuffer------------------

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size, uint32_t drawMode) {
		glCreateBuffers(1, &m_BufferId);
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMode);
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer() {
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLVertexBuffer::bind() const {
		glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLVertexBuffer::unbind() const {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// -------------IndexBuffer------------------

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count) :
		m_Count(count) {

		glCreateBuffers(1, &m_BufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer() {
		glDeleteBuffers(1, &m_BufferId);
	}

	void OpenGLIndexBuffer::bind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	}

	void OpenGLIndexBuffer::unbind() const {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// -------------Framebuffer------------------

	/*OpenGLFramebuffer::OpenGLFramebuffer(uint32_t width, uint32_t height) :
		m_Width(width),
		m_Height(height) {
		uint32_t fbo;
		glGenFramebuffers(1, &fbo);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		uint32_t colorId;
		glGenTextures(1, &colorId);
		glBindTexture(GL_TEXTURE_2D, colorId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorId, 0);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			SLD_CORE_ASSERT_FAIL("Error while creating a frame buffer object!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_FramebufferId = fbo;
		//m_RenderBufferId = rbo;
		m_ColorBuffer = createRef<OpenGLTexture2D>(colorId, m_Width, m_Height, GL_RGBA, GL_RGBA);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {

	}

	void OpenGLFramebuffer::bind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FramebufferId);
	}

	void OpenGLFramebuffer::unbind() const {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}*/
}