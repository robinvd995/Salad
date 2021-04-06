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
		m_Position.x += x;
		m_Position.y += y;
		m_Position.z += z;
	}

	/*void Transform::setRotation(float x, float y, float z) {
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	}

	void Transform::rotate(float x, float y, float z) {
		m_Rotation.x += x;
		m_Rotation.y += y;
		m_Rotation.z += z;
	}*/

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
		m_TransformMatrix = glm::mat4(1.0f);
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