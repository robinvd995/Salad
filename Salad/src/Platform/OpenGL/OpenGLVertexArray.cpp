#include "sldpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Salad {

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:		return GL_FLOAT;
			case ShaderDataType::Float2:	return GL_FLOAT;
			case ShaderDataType::Float3:	return GL_FLOAT;
			case ShaderDataType::Float4:	return GL_FLOAT;
			case ShaderDataType::Mat3:		return GL_FLOAT;
			case ShaderDataType::Mat4:		return GL_FLOAT;
			case ShaderDataType::Int:		return GL_INT;
			case ShaderDataType::Int2:		return GL_INT;
			case ShaderDataType::Int3:		return GL_INT;
			case ShaderDataType::Int4:		return GL_INT;
			case ShaderDataType::Bool:		return GL_BOOL;
		}

		SLD_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_VertexArrayId);
	}

	void OpenGLVertexArray::bind() const {
		glBindVertexArray(m_VertexArrayId);
	}

	void OpenGLVertexArray::unbind() const {
		glBindVertexArray(0);
	}
	
	void OpenGLVertexArray::addVertexBuffer(const Ref<VertexBuffer>& vertexbuffer) {
		SLD_CORE_ASSERT(vertexbuffer->getLayout().getElements().size(), "Vertex Buffer has no layout!");
		
		glBindVertexArray(m_VertexArrayId);
		vertexbuffer->bind();

		uint32_t index = 0;
		const BufferLayout& layout = vertexbuffer->getLayout();
		for (const BufferElement& element : layout) {
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.getComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.type),
				element.normalized ? GL_TRUE : GL_FALSE,
				layout.getStride(),
				(const void*)element.offset);
			index++;
		}

		m_VertexBuffers.push_back(vertexbuffer);
	}

	void OpenGLVertexArray::setIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_VertexArrayId);
		indexBuffer->bind();

		m_IndexBuffer = indexBuffer;
	}
}