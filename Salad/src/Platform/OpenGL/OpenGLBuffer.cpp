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

	// -------------UniformBuffer------------------

	OpenGLUniformBuffer::OpenGLUniformBuffer(const uint32_t binding, const uint32_t size) :
		UniformBuffer(binding, size)
	{
		glGenBuffers(1, &m_BufferId);
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
		glBufferData(GL_UNIFORM_BUFFER, c_BufferSize, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindBufferRange(GL_UNIFORM_BUFFER, c_Binding, m_BufferId, 0, c_BufferSize);
	}

	void OpenGLUniformBuffer::updateBuffer(const void* data) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, c_BufferSize, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void OpenGLUniformBuffer::updateBuffer(const uint32_t offset, const uint32_t size, const void* data) {
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferId);
		glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}