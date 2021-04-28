#include "sldpch.h"
#include "Texture.h"

#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Salad {
	Ref<Texture2D> Texture2D::create(const std::string& filepath, TextureFilterWrapSpecification filterWrapSpec) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(filepath, filterWrapSpec);
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

	Ref<Texture2D> Texture2D::create(uint32_t width, uint32_t height, unsigned char* data) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(width, height, data);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::create(TextureSpecification& specs, unsigned char* data) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLTexture2D>(specs, data);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}

	Ref<TextureCubeMap> TextureCubeMap::create(const std::string& filepath, const std::string& extension) {
		switch (Renderer::getAPI()) {
			case RenderAPI::API::NONE: SLD_CORE_ASSERT(false, "Render API None is not supported!"); return nullptr;
			case RenderAPI::API::OpenGL: return createRef<OpenGLCubeMap>(filepath, extension);
		}

		SLD_CORE_ASSERT(false, "Unknown renderer API!");
		return nullptr;
	}
}
