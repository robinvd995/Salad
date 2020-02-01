#include "sldpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Salad {
	Ref<Texture2D> Texture2D::create(const std::string& filepath) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(filepath);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(width, height);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}