#include "sldpch.h"
#include "Transform.h"
#include <glm/ext/matrix_transform.hpp>

namespace Salad {

	void Transform::setPosition(float x, float y, float z) {
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}

	void Transform::translate(float x, float y, float z) {
		translate(x, glm::vec3(1.0f, 0.0f, 0.0f));
		translate(y, glm::vec3(0.0f, 1.0f, 0.0f));
		translate(z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Transform::translate(glm::vec3& distances) {
		translate(distances.x, glm::vec3(1.0f, 0.0f, 0.0f));
		translate(distances.y, glm::vec3(0.0f, 1.0f, 0.0f));
		translate(distances.z, glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Transform::translate(float distance, glm::vec3& axis) {
		glm::vec3 direction = m_Orientation * axis;

		m_Position.x += direction.x * distance;
		m_Position.y += direction.y * distance;
		m_Position.z += direction.z * distance;
	}

	void Transform::translateForward(float distance) {
		/*float x = 2 * (m_Orientation.x * m_Orientation.z + m_Orientation.w * m_Orientation.y);
		float y = 2 * (m_Orientation.y * m_Orientation.z + m_Orientation.w * m_Orientation.x);
		float z = 1 - 2 * (m_Orientation.x * m_Orientation.x + m_Orientation.y * m_Orientation.y);
		translate(x * amount, y * amount, z * amount);*/
		glm::vec3 direction = m_Orientation * glm::vec3(0.0f, 0.0f, 1.0f);
		//translate(direction.x * amount, direction.y * amount, direction.z * amount);

		m_Position.x += direction.x * distance;
		m_Position.y += direction.y * distance;
		m_Position.z += direction.z * distance;
	}

	void Transform::translateUp(float distance) {
		/*float x = 2 * (m_Orientation.x * m_Orientation.y - m_Orientation.w * m_Orientation.z);
		float y = 1 - 2 * (m_Orientation.x * m_Orientation.x + m_Orientation.z * m_Orientation.z);
		float z = 2 * (m_Orientation.y * m_Orientation.z + m_Orientation.w * m_Orientation.x);
		translate(x * amount, y * amount, z * amount);*/
		glm::vec3 direction = m_Orientation * glm::vec3(0.0f, 1.0f, 0.0f);
		//translate(direction.x * amount, direction.y * amount, direction.z * amount);

		m_Position.x += direction.x * distance;
		m_Position.y += direction.y * distance;
		m_Position.z += direction.z * distance;
	}

	void Transform::translateLeft(float distance) {
		/*float x = 1 - 2 * (m_Orientation.y * m_Orientation.y + m_Orientation.z * m_Orientation.z);
		float y = 2 * (m_Orientation.x * m_Orientation.y + m_Orientation.w * m_Orientation.z);
		float z = 2 * (m_Orientation.x * m_Orientation.z - m_Orientation.w * m_Orientation.y);
		translate(x * amount, y * amount, z * amount);*/
		glm::vec3 direction = m_Orientation * glm::vec3(1.0f, 0.0f, 0.0f);
		//translate(direction.x * amount, direction.y * amount, direction.z * amount);

		m_Position.x += direction.x * distance;
		m_Position.y += direction.y * distance;
		m_Position.z += direction.z * distance;
	}

	/*void Transform::setRotation(float x, float y, float z) {
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	}*/

	void Transform::rotate(float angle, glm::vec3& axis) {
		glm::quat rot = glm::angleAxis(angle, axis);
		m_Orientation *= rot;

		//m_Orientation = glm::rotate(m_Orientation, angle, axis);
		//m_Orientation = identity;
	}

	void Transform::setScale(float x, float y, float z) {
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}

	void Transform::scale(float x, float y, float z) {
		m_Scale.x += x;
		m_Scale.y += y;
		m_Scale.z += z;
	}

	void Transform::setValues(glm::vec3& position, glm::quat& orientation, glm::vec3& scale) {
		m_Position.x = position.x;
		m_Position.y = position.y;
		m_Position.z = position.z;

		m_Orientation = orientation;

		/*m_Rotation.x = rotation.x;
		m_Rotation.y = rotation.y;
		m_Rotation.z = rotation.z;*/

		m_Scale.x = scale.x;
		m_Scale.y = scale.y;
		m_Scale.z = scale.z;
	}

	void Transform::calculateTransformationMatrix() {
		m_TransformMatrix = glm::identity<glm::mat4>();
		m_TransformMatrix = glm::translate(m_TransformMatrix, m_Position);
		m_TransformMatrix *= glm::mat4_cast(m_Orientation);
		/*m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_TransformMatrix = glm::rotate(m_TransformMatrix, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));*/
		m_TransformMatrix = glm::scale(m_TransformMatrix, m_Scale);
	}


	/*void Transform::calculateWorldSpaceMatrix() {
		m_WorldSpaceMatrix = glm::translate(m_TransformMatrix, );
	}*/

}