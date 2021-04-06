#pragma once

#include <glm/glm.hpp>

namespace Salad{

	class Transform {
	public:
		Transform() :
			m_Position(0.0f, 0.0f, 0.1f),
			m_Rotation(0.0f, 0.0f, 0.0f),
			m_Scale(1.0f, 1.0f, 1.0f) {
		}

		Transform(const Transform& other) = delete;

		~Transform() {}

		inline glm::vec3& getPosition() { return m_Position; }
		inline glm::vec3& getRotation() { return m_Rotation; }
		inline glm::vec3& getScale() { return m_Scale; };

		inline glm::vec3 cpyPosition() const { return m_Position; }
		inline glm::vec3 cpyRotation() const { return m_Rotation; }
		inline glm::vec3 cpyScale() const { return m_Scale; };

		void setPosition(float x, float y, float z);
		void setPosition(glm::vec3& position);

		void setRotation(float x, float y, float z);
		void setRotation(glm::vec3& rotation);

		void setScale(float x, float y, float z);
		void setScale(glm::vec3& scale);
		
		void translate(float x, float y);
		void translate(const glm::vec2& delta);
		void translate(float x, float y, float z);
		void translate(const glm::vec3& delta);

		void rotate(float x, float y, float z);
		void rotate(float angle, glm::vec3 rotation);

		glm::mat4 getTransformationMatrix();

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
	};

}

