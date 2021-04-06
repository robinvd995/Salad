#pragma once

namespace Salad {

	struct FramebufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		// True if rendering to screen, false if not
		bool SwapChainTarget = false;
	};

	class Framebuffer {
	
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		virtual uint32_t getColorAttachment() const = 0;

		static Ref<Framebuffer> create( const FramebufferSpecification& spec);

	};

}
