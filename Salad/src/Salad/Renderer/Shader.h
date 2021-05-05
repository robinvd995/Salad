#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

namespace Salad {

	class Shader {

	public:
		~Shader() = default;

		virtual void bind() const = 0;
		virtual void unbind() const = 0;

		virtual const std::string& getName() const = 0;

		virtual void setInt(const std::string& name, const int value) = 0;

		virtual void setFloat(const std::string& name, const float value) = 0;
		virtual void setFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void setFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void setFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void setMat3(const std::string& name, const glm::mat3& value) = 0;
		virtual void setMat4(const std::string& name, const glm::mat4& value) = 0;

		virtual uint32_t getProgramId() = 0;

		static Ref<Shader> create(const std::string& filepath);
		static Ref<Shader> create(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
	};

	class ShaderLibrary {
	public:
		void add(const std::string& name, const Ref<Shader>& shader);
		void add(const Ref<Shader>& shader);
		Ref<Shader> load(const std::string& filepath);
		Ref<Shader> load(const std::string& name, const std::string& filepath);

		Ref<Shader> get(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}