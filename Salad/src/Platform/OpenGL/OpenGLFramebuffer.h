#pragma once

#include "Salad/Renderer/Framebuffer.h"

namespace Salad {

	class OpenGLFramebuffer : public Framebuffer {
	
	public:

		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

		void invalidate();

		virtual void bind() override;
		virtual void unbind() override;

		virtual void resize(uint32_t width, uint32_t height) override;

		virtual int readPixel(uint32_t index, uint32_t x, uint32_t y) override;
		virtual void clearColorAttachment(uint32_t colorAttachmentId, const void* data) override;
		virtual void clearColorBuffer(uint32_t bufferIndex, const void* data) override;

		virtual const FramebufferSpecification& getSpecification() const override { return m_Spec; }

		virtual uint32_t getColorAttachment(uint32_t index = 0) const override { SLD_CORE_ASSERT((index < m_ColorAttachments.size()), ""); return m_ColorAttachments[index]; }

	private:
		uint32_t m_RendererId = 0;

		FramebufferSpecification m_Spec;

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;
		FramebufferTextureSpecification m_DepthAttachmentSpec;

		std::vector<uint32_t> m_ColorAttachments;
		uint32_t m_DepthAttachment;
	};

}
