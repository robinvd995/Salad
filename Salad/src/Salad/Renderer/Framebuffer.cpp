#include "sldpch.h"
#include "Framebuffer.h"

#include "Renderer.h"

#include "Platform/OpenGL/OpenGLFramebuffer.h"

namespace Salad {

	Ref<Framebuffer> Framebuffer::create(const FramebufferSpecification& spec) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return std::make_shared<OpenGLFramebuffer>(spec);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

}