#include "sldpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Salad {

	static GLenum shaderTypeFromString(const std::string& type) {
		if (type == "vertex") {
			return GL_VERTEX_SHADER;
		}
		else if (type == "fragment" || type == "pixel") {
			return GL_FRAGMENT_SHADER;
		}
		SLD_CORE_ERROR("Shader Type:'{0}'", type);

		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath) {
		std::string shaderSource = readFile(filepath);
		auto shaderSources = preProcess(shaderSource);
		compile(shaderSources);

		//
		size_t lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		size_t lastDot = filepath.rfind(".");
		size_t count = (lastDot == std::string::npos ? filepath.size() : lastDot) - lastSlash;

		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource) :
		m_Name(name)
	{
		std::unordered_map<GLenum, std::string> shaderSource;
		shaderSource.insert(std::make_pair(GL_VERTEX_SHADER, vertexSource));
		shaderSource.insert(std::make_pair(GL_FRAGMENT_SHADER, fragmentSource));
		compile(shaderSource);
	}

	OpenGLShader::OpenGLShader(const std::string& name, Util::ByteBuffer* buffer) :
		m_Name(name)
	{
		//buffer->skip<uint64_t>(0);
		buffer->moveIterator(0);
		uint64_t openglindex = buffer->read<uint64_t>();

		// uint64_t directxindex = buffer->read<uint64_t>();
		// uint64_t vulkanindex = buffer->read<uint64_t>();
		// uint64_t metalindex = buffer->read<uint64_t>();

		SLD_CORE_ASSERT(openglindex > 0, "Unable to load shader, index = 0!");
		buffer->moveIterator(openglindex);

		std::vector<uint32_t> shaderIds;
		const static GLenum stageMap[4] = { GL_VERTEX_SHADER, GL_GEOMETRY_SHADER, GL_FRAGMENT_SHADER, GL_COMPUTE_SHADER };

		for(int i = 0; i < 4; i++) {
			uint32_t stagesize = buffer->read<uint32_t>();
			if (stagesize == 0) continue;

			GLenum stage = GL_VERTEX_SHADER;
			switch(i){
				case 0: stage = GL_VERTEX_SHADER; break;
				case 1: stage = GL_GEOMETRY_SHADER; break;
				case 2: stage = GL_FRAGMENT_SHADER; break;
				case 3: stage = GL_COMPUTE_SHADER; break;
			}

			uint32_t shaderid = glCreateShader(stage);
			char* stageSource = buffer->readArray<char>(stagesize);

			glShaderSource(shaderid, 1, &stageSource, 0);
			glCompileShader(shaderid);

			int compileStatus = 0;
			glGetShaderiv(shaderid, GL_COMPILE_STATUS, &compileStatus);
			if (compileStatus == GL_FALSE) 			{
				GLint maxLength = 500;
				glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shaderid, maxLength, &maxLength, &infoLog[0]);
				glDeleteShader(shaderid);

				SLD_CORE_ERROR("{0}", infoLog.data());
				SLD_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}
			else {
				shaderIds.push_back(shaderid);
			}
		}

		GLuint program = glCreateProgram();
		for(uint32_t shaderid : shaderIds) glAttachShader(program, shaderid);

		glLinkProgram(program);
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE) 		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			for (uint32_t shaderid : shaderIds) glDeleteShader(shaderid);

			SLD_CORE_ERROR("{0}", infoLog.data());
			SLD_CORE_ASSERT(false, "Shader link failure!");

			return;
		}

		for (uint32_t shaderid : shaderIds) glDetachShader(program, shaderid);
		m_ShaderId = program;
	}

	std::string OpenGLShader::readFile(const std::string& filepath) {
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		std::string result;
		if (in) {
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else {
			SLD_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(const std::string& shaderSource) {
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* eolToken = "\r\n";
		const char preProcessorToken = '$';
		const int ppTokenLength = 1;
		
		size_t curPos = 0;

		GLenum shaderType = 0;
		bool hasShaderType = false;

		while (curPos != std::string::npos) {

			size_t eol = shaderSource.find(eolToken, curPos);
			std::string line = shaderSource.substr(curPos, eol - curPos);

			char firstChar = line[0];

			if (firstChar == preProcessorToken) {
				size_t ppdEnd = line.find(' ', ppTokenLength);
				std::string directive = line.substr(ppTokenLength, ppdEnd - ppTokenLength);
				std::string argument = line.substr(ppdEnd + 1, eol - ppdEnd + 1);

				//PREPROCESSOR STUFF
				bool addToSource = true;
				if (directive == "type") {
					shaderType = shaderTypeFromString(argument);
					hasShaderType = true;
					shaderSources.insert(std::make_pair(shaderType, std::string("")));
				}

				else if (directive == "add_uniform") {
					shaderSources[shaderType] += (argument + eolToken);
				}
			}
			else if(hasShaderType){
				shaderSources[shaderType] += (line + eolToken);
			}

			curPos = shaderSource.find_first_not_of(eolToken, eol);
		}

		for (auto& key : shaderSources) {
			const std::string& s = key.second;
		}

		return shaderSources;
	}

	void OpenGLShader::compile(const std::unordered_map<GLenum, std::string>& shaderSources) {

		int sourcesAmount = shaderSources.size();
		GLuint program = glCreateProgram();

		SLD_CORE_ASSERT(sourcesAmount > 4, "Shader sources cannot exceed 4!");
		std::array<GLuint, 4> shaderIds;

		SLD_CORE_ASSERT(shaderSources.size(), "No shader sources passed in to compile!");

		int shaderIdIndex = 0;
		for (auto& key : shaderSources) {
			GLenum shaderType = key.first;
			const std::string& shaderSource = key.second;

			GLuint shader = glCreateShader(shaderType);
			const GLchar* source = (const GLchar*)shaderSource.c_str();

			glShaderSource(shader, 1, &source, 0);
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 500;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				SLD_CORE_ERROR("{0}", infoLog.data());
				SLD_CORE_ASSERT(false, "Shader compilation failure!");

				break;
			}
			
			glAttachShader(program, shader);
			shaderIds[shaderIdIndex++] = shader;
		}

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);
			
			for (GLuint i = 0; i < sourcesAmount; i++)
				glDeleteShader(shaderIds[i]);

			SLD_CORE_ERROR("{0}", infoLog.data());
			SLD_CORE_ASSERT(false, "Shader link failure!");
		}

		for (GLuint i = 0; i < sourcesAmount; i++)
			glDetachShader(program, shaderIds[i]);

		m_ShaderId = program;
	}

	OpenGLShader::~OpenGLShader() {
		glDeleteProgram(m_ShaderId);
	}

	void OpenGLShader::bind() const {
		glUseProgram(m_ShaderId);
	}

	void OpenGLShader::unbind() const {
		glUseProgram(0);
	}

	void OpenGLShader::setInt(const std::string& name, const int value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform1iv(location, 1, &value);
	}

	void OpenGLShader::setFloat(const std::string& name, const float value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform1f(location, value);
	}

	void OpenGLShader::setFloat2(const std::string& name, const glm::vec2& value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void OpenGLShader::setFloat3(const std::string& name, const glm::vec3& value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::setFloat4(const std::string& name, const glm::vec4& value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::setMat3(const std::string& name, const glm::mat3& value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	void OpenGLShader::setMat4(const std::string& name, const glm::mat4& value) {
		GLuint location = glGetUniformLocation(m_ShaderId, name.c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}

	/*uint32_t OpenGLShader::getUniformBlockIndex(const std::string name) {
		GLuint location = glGetUniformBlockIndex(m_ShaderId, name.c_str());
		return location;
	}*/
}
