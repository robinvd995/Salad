#pragma once

namespace Salad {

	enum class FramebufferTextureFormat {
	
		None = 0,

		// Colors
		RGBA8,

		// Data
		RED_INTEGER,

		// Depth/Stencil
		DEPTH24STENCIL8
	
	};

	struct FramebufferTextureSpecification {
	
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) :
			textureFormat(format)
		{}

		FramebufferTextureFormat textureFormat = FramebufferTextureFormat::None;
	};

	struct FrambufferAttachmentSpecification {

		FrambufferAttachmentSpecification() = default;
		FrambufferAttachmentSpecification(std::initializer_list< FramebufferTextureSpecification> p_attachments) :
			attachments(p_attachments)
		{}

		std::vector<FramebufferTextureSpecification> attachments;
	};

	struct FramebufferSpecification {
		uint32_t Width, Height;
		uint32_t Samples = 1;

		FrambufferAttachmentSpecification attachments;

		// True if rendering to screen, false if not
		bool SwapChainTarget = false;
	};

	class Framebuffer {
	
	public:
		virtual ~Framebuffer() = default;

		virtual void bind() = 0;
		virtual void unbind() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual int readPixel(uint32_t index, uint32_t x, uint32_t y) = 0;
		virtual void clearColorAttachment(uint32_t colorAttachmentId, const void* data) = 0;
		virtual void clearColorBuffer(uint32_t bufferIndex, const void* data) = 0;

		virtual const FramebufferSpecification& getSpecification() const = 0;

		virtual uint32_t getColorAttachment(uint32_t index = 0) const = 0;

		static Ref<Framebuffer> create( const FramebufferSpecification& spec);

	};

}
