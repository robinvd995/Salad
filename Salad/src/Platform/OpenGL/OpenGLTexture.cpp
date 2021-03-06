#include "sldpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

namespace Salad {

	// ----- Texture2D -----

	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height) :
		m_Width(width),
		m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureId);
		glTextureStorage2D(m_TextureId, 1, m_InternalFormat, m_Width, m_Height);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath) :
		m_FilePath(filepath),
		m_Width(0),
		m_Height(0)
	{
		int width, height, channels;
		//stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
		SLD_CORE_ASSERT(data, "Failed to load image!");

		m_Width = width;
		m_Height = height;

		if (channels == 4) {
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}

		SLD_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Texture format not supported!");

		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	OpenGLTexture2D::OpenGLTexture2D(uint32_t textureId, uint32_t width, uint32_t height, GLenum internalFormat, GLenum dataFormat) :
		m_Width(width),
		m_Height(height),
		m_TextureId(textureId),
		m_InternalFormat(internalFormat),
		m_DataFormat(dataFormat)
	{}

	void OpenGLTexture2D::setData(void* data, uint32_t size) {
		glTextureSubImage2D(m_TextureId, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		//SLD_CORE_INFO("Texture deleted!");
		glDeleteTextures(1, &m_TextureId);
	}

	void OpenGLTexture2D::bind(uint32_t slot) const {
		uint32_t textureUnit = GL_TEXTURE0 + slot;
		glActiveTexture(textureUnit);
		//glBindTextureUnit(textureUnit, m_TextureId);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
	}

	// ----- Texture2D END ----- 

	// ----- Cube Map -----

	std::string OpenGLCubeMap::texture_faces[] = { "right", "left", "top", "bottom", "back", "front" };

	OpenGLCubeMap::OpenGLCubeMap(const std::string& filepath, const std::string& extension) :
		m_InternalFormat(GL_RGBA8),
		m_DataFormat(GL_RGBA),
		m_Width(0), m_Height(0)
	{
		glGenTextures(1, &m_TextureId);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);

		int width, height, channels;
		stbi_uc* data;

		for(unsigned int i = 0; i < 6; i++) {
			std::string filename = filepath + (std::string)("_") + (texture_faces[i]) + extension;
			data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

			if(data){
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, m_InternalFormat, width, height, 0, m_DataFormat, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else{
				SLD_CORE_ERROR("Cubemap texture failed to load at path: {0}", filepath + "_" + texture_faces[i] + extension);
			}
			
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}

	void OpenGLCubeMap::bind(uint32_t slot) const {
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_TextureId);
	}

	OpenGLCubeMap::~OpenGLCubeMap() {
		
	}

	// ----- Cube Map END -----
}