#include "sldpch.h"
#include "Buffer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Renderer.h"

namespace Salad {

	Ref<VertexBuffer> VertexBuffer::create(float* vertices, uint32_t size) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexBuffer>(vertices, size);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::create(uint32_t* indices, uint32_t size) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return std::make_shared<OpenGLIndexBuffer>(indices, size);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}