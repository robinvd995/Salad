#pragma once

#include "Salad/Renderer/Shader.h"

// TODO: REMOVE!!!!
typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace Salad {

	class OpenGLShader : public Shader{

	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		~OpenGLShader();

		virtual void bind() const override;
		virtual void unbind() const override;

		virtual const std::string& getName() const override { return m_Name; }

		virtual void setInt(const std::string& name, const int value) override;

		virtual void setFloat(const std::string& name, const float value) override;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) override;

		virtual void setMat3(const std::string& name, const glm::mat3& value) override;
		virtual void setMat4(const std::string& name, const glm::mat4& value) override;

	private:
		uint32_t m_ShaderId;
		std::string m_Name;
	private:
		std::string readFile(const std::string& filepath);
		std::unordered_map<GLenum, std::string> preProcess(const std::string& shaderSource);
		void compile(const std::unordered_map<GLenum, std::string>& shaderSources);
	};

}
