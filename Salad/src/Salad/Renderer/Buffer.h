#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Renderer/Texture.h"

#include <string>

namespace Salad {

	enum class ShaderDataType {
		None = 0, 
		Float, Float2, Float3, Float4, 
		Mat3, Mat4, 
		Int, Int2, Int3, Int4,
		Bool
	};

	static uint32_t shaderDataTypeSize(ShaderDataType type) {
		switch (type) {
			case ShaderDataType::Float:		return 4;
			case ShaderDataType::Float2:	return 4 * 2;
			case ShaderDataType::Float3:	return 4 * 3;
			case ShaderDataType::Float4:	return 4 * 4;
			case ShaderDataType::Mat3:		return 4 * 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4 * 4;
			case ShaderDataType::Int:		return 4;
			case ShaderDataType::Int2:		return 4 * 2;
			case ShaderDataType::Int3:		return 4 * 3;
			case ShaderDataType::Int4:		return 4 * 4;
			case ShaderDataType::Bool:		return 1;
		}

		SLD_CORE_ASSERT(false, "Unknown shader data type!");
		return 0;
	}

	struct BufferElement {
		std::string name;
		ShaderDataType type;
		uint32_t size;
		uint32_t offset;
		bool normalized;

		BufferElement() {}
		BufferElement(const std::string pname, ShaderDataType ptype, bool pnormalized = false) :
			name(pname),
			type(ptype),
			size(shaderDataTypeSize(ptype)),
			offset(0),
			normalized(pnormalized)
		{}

		uint32_t getComponentCount() const {
			switch (type) {
				case ShaderDataType::Float:		return 1;
				case ShaderDataType::Float2:	return 2;
				case ShaderDataType::Float3:	return 3;
				case ShaderDataType::Float4:	return 4;
				case ShaderDataType::Mat3:		return 3 * 3;
				case ShaderDataType::Mat4:		return 4 * 4;
				case ShaderDataType::Int:		return 1;
				case ShaderDataType::Int2:		return 2;
				case ShaderDataType::Int3:		return 3;
				case ShaderDataType::Int4:		return 4;
				case ShaderDataType::Bool:		return 1;
			}

			SLD_CORE_ASSERT(false, "Unknown shader data type!");
			return 0;
		}
	};

	class BufferLayout {
	
	public:
		BufferLayout() {}
		BufferLayout(const std::initializer_list<BufferElement>& elements) :
			m_Elements(elements) {
			calculateOffsetAndStride();
		}

		inline const std::vector<BufferElement>& getElements() const { return m_Elements; }
		inline const uint32_t getStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void calculateOffsetAndStride() {

			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.offset = offset;
				offset += element.size;
				m_Stride += element.size;
			}
		}

	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual void setLayout(const BufferLayout layout) = 0;
		virtual const BufferLayout& getLayout() = 0;

		static Ref<VertexBuffer> create(float* vertices, uint32_t size, uint32_t drawMode);
	};

	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual uint32_t getCount() const = 0;

		static Ref<IndexBuffer> create(uint32_t* indices, uint32_t size);
	};

	/*class Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual Ref<Texture2D> getColorBuffer() const = 0;

		static Ref<Framebuffer> create(uint32_t width, uint32_t height);
	};*/

}