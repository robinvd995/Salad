#include "sldpch.h"
#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Salad {

	void Transform::setPosition(float x, float y, float z) {
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}

	void Transform::setPosition(glm::vec3& position) {
		m_Position.x = position.x;
		m_Position.y = position.y;
		m_Position.z = position.z;
	}

	void Transform::setRotation(float x, float y, float z) {
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	}

	void Transform::setRotation(glm::vec3& rotation) {
		m_Rotation.x = rotation.x;
		m_Rotation.y = rotation.y;
		m_Rotation.z = rotation.z;
	}

	void Transform::setScale(float x, float y, float z) {
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}

	void Transform::setScale(glm::vec3& scale) {
		m_Scale.x = scale.x;
		m_Scale.y = scale.y;
		m_Scale.z = scale.z;
	}

	void Transform::translate(float x, float y) {
		m_Position.x += x;
		m_Position.y += y;
	}

	void Transform::translate(const glm::vec2& delta) {
		m_Position.x += delta.x;
		m_Position.y += delta.y;
	}

	void Transform::translate(float x, float y, float z) {
		m_Position.x += x;
		m_Position.y += y;
		m_Position.z += z;
	}

	void Transform::translate(const glm::vec3& delta) {
		m_Position.x += delta.x;
		m_Position.y += delta.y;
		m_Position.z += delta.z;
	}

	void Transform::rotate(float x, float y, float z) {
		m_Rotation.x += x;
		m_Rotation.y += y;
		m_Rotation.z += z;
	}

	void Transform::rotate(float angle, glm::vec3 rotation) {
		m_Rotation += angle * rotation;
	}

	glm::mat4 Transform::getTransformationMatrix(){
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, m_Position);
		transform = glm::rotate(transform, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		transform = glm::rotate(transform, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		transform = glm::rotate(transform, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		transform = glm::scale(transform, m_Scale);
		return transform;
	}

}
