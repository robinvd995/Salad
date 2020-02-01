#include "sldpch.h"

#include "VertexArray.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Salad {

	Ref<VertexArray>VertexArray::create() {
		switch (Renderer::getAPI()) {
		case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
		case RenderAPI::API::OpenGL: return std::make_shared<OpenGLVertexArray>();
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

}