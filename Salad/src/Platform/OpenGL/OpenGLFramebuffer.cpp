#include "sldpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

#define MAX_COLOR_ATTACHMENTS 4

namespace Salad {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Util {

		static bool isDepthFormat(FramebufferTextureFormat format) {
			switch (format) {
				case Salad::FramebufferTextureFormat::DEPTH24STENCIL8: return true;
			}
			return false;
		}

		static GLenum textureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void createTextures(bool multisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(textureTarget(multisampled), count, outID);
		}

		static void bindTexture(bool multisampled, uint32_t textureid) {
			glBindTexture(textureTarget(multisampled), textureid);
		}

		static void attachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
		
			bool multisampled = samples > 1;
			if(multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				//Get filters/wraps from framebufffer spec
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, textureTarget(multisampled), id, 0);
		}

		static void attachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				//Get filters/wraps from framebufffer spec
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, textureTarget(multisampled), id, 0);
		}

		static GLenum framebufferTextureFormatToGL(FramebufferTextureFormat format) {
			switch(format) {
				case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}
			SLD_CORE_ASSERT_FAIL("Unknown format!");
			return 0;
		}

		static GLenum glDataType(FramebufferTextureFormat format) {
			switch (format) {
				case FramebufferTextureFormat::RGBA8: return GL_UNSIGNED_BYTE;
				case FramebufferTextureFormat::RED_INTEGER: return GL_INT;
			}
			SLD_CORE_ASSERT_FAIL("Unknown format!");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Spec(spec)
	{
		for(auto format : m_Spec.attachments.attachments) {
			if(!Util::isDepthFormat(format.textureFormat)) 
				m_ColorAttachmentSpecs.push_back(format);
			else
				m_DepthAttachmentSpec = format;
		}

		invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererId);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::invalidate(){

		if(m_RendererId != 0){
			glDeleteFramebuffers(1, &m_RendererId);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
		}

		glCreateFramebuffers(1, &m_RendererId);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);

		// Attachments

		bool multisampled = m_Spec.Samples > 1;

		if(m_ColorAttachmentSpecs.size() > 0) {

			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());
			Util::createTextures(multisampled, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
				Util::bindTexture(multisampled, m_ColorAttachments[i]);
				switch(m_ColorAttachmentSpecs[i].textureFormat) {
					case FramebufferTextureFormat::RGBA8:
						Util::attachColorTexture(m_ColorAttachments[i], m_Spec.Samples, GL_RGBA8, GL_RGBA, m_Spec.Width, m_Spec.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Util::attachColorTexture(m_ColorAttachments[i], m_Spec.Samples, GL_R32I, GL_RED_INTEGER, m_Spec.Width, m_Spec.Height, i);
						break;
				}
			}
		}
		
		if(m_DepthAttachmentSpec.textureFormat != FramebufferTextureFormat::None) {
		
			Util::createTextures(multisampled, &m_DepthAttachment, 1);
			Util::bindTexture(multisampled, m_DepthAttachment);
			switch(m_DepthAttachmentSpec.textureFormat) {
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Util::attachDepthTexture(m_DepthAttachment, m_Spec.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Spec.Width, m_Spec.Height);
					break;
			
			}
		}

		if(m_ColorAttachments.size() > 1) {
		
			SLD_CORE_ASSERT((m_ColorAttachments.size() <= MAX_COLOR_ATTACHMENTS), "Too many color attachments attached to the framebuffer!");
			GLenum buffers[MAX_COLOR_ATTACHMENTS] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if(m_ColorAttachments.empty()) {
			glDrawBuffer(GL_NONE);
		}

		SLD_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererId);
		glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	}

	void OpenGLFramebuffer::unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::resize(uint32_t width, uint32_t height) {

		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) 		{
			SLD_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Spec.Width = width;
		m_Spec.Height = height;
		invalidate();
	}

	int OpenGLFramebuffer::readPixel(uint32_t index, uint32_t x, uint32_t y) {
		SLD_CORE_ASSERT((index <= m_ColorAttachments.size()), "Trying to get pixel outside of frame buffer range!");
		glReadBuffer(GL_COLOR_ATTACHMENT0 + index);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;
	}

	void OpenGLFramebuffer::clearColorAttachment(uint32_t index, const void* data) {
		SLD_CORE_ASSERT((index <= m_ColorAttachments.size()), "Trying clear color attachment outside of frame buffer range!");
		auto& spec = m_ColorAttachmentSpecs[index];
		GLenum format = Util::framebufferTextureFormatToGL(spec.textureFormat);
		GLenum datatype = Util::glDataType(spec.textureFormat);
		glClearTexImage(m_ColorAttachments[index], 0, format, datatype, data);
	}

	void OpenGLFramebuffer::clearColorBuffer(uint32_t bufferIndex, const void* data) {
		glClearBufferfv(GL_COLOR, bufferIndex, (float*)data);
	}
}
