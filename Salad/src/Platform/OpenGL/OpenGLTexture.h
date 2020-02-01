#pragma once

#include "Salad/Renderer/Texture.h"

#include <glad/glad.h>

namespace Salad {

	class OpenGLTexture2D : public Texture2D {

	public:
		virtual ~OpenGLTexture2D();

		OpenGLTexture2D(const std::string& filepath);
		OpenGLTexture2D(uint32_t width, uint32_t height);

		virtual void setData(void* data, uint32_t size) override;

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }

		virtual void bind(uint32_t slot) const override;

	private:
		std::string m_FilePath;
		uint32_t m_Width, m_Height = 0;
		uint32_t m_TextureId;

		GLenum m_InternalFormat, m_DataFormat;
	};

}