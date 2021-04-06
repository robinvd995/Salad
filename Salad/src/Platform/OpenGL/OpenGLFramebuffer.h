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

		virtual const FramebufferSpecification& getSpecification() const override { return m_Spec; }

		virtual uint32_t getColorAttachment() const override { return m_ColorAttachment; }

	private:
		uint32_t m_RendererId = 0;
		uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

		FramebufferSpecification m_Spec;

	};

}