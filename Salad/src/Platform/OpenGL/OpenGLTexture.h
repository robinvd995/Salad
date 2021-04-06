#pragma once

#include "Salad/Renderer/Texture.h"

#include <glad/glad.h>

namespace Salad {

	class OpenGLTexture2D : public Texture2D {

	public:
		virtual ~OpenGLTexture2D();

		OpenGLTexture2D(const std::string& filepath);
		OpenGLTexture2D(uint32_t width, uint32_t height);
		OpenGLTexture2D(uint32_t textureId, uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat);

		virtual void setData(void* data, uint32_t size) override;

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }
		virtual uint32_t getRendererId() const override { return m_TextureId; }

		virtual void bind(uint32_t slot) const override;

	private:
		std::string m_FilePath;
		uint32_t m_Width, m_Height = 0;
		uint32_t m_TextureId;

		GLenum m_InternalFormat, m_DataFormat;
	};

	class OpenGLCubeMap : public TextureCubeMap {
	
	public:

		virtual ~OpenGLCubeMap();

		OpenGLCubeMap(const std::string& filepath, const std::string& extension);

		virtual void setData(void* data, uint32_t size) override {};

		virtual uint32_t getWidth() const override { return m_Width; }
		virtual uint32_t getHeight() const override { return m_Height; }
		virtual uint32_t getRendererId() const override { return m_TextureId; }

		virtual void bind(uint32_t slot) const override;

	private:
		std::string m_FilePath;
		uint32_t m_Width, m_Height;
		uint32_t m_TextureId;

		GLenum m_InternalFormat, m_DataFormat;
	
		static std::string texture_faces[];
	};

}