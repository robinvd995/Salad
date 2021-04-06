#include "sldpch.h"
#include "EntityTransform.h"

namespace Salad {

	glm::vec3 EntityTransform::getWorldSpacePosition(EntityTransform& other) {
		return other.getPosition() + getPosition();
	}

	glm::vec3 EntityTransform::getWorldSpaceRotation(EntityTransform& other) {
		return other.getRotation() + getRotation();
	}

	glm::vec3 EntityTransform::getWorldSpaceScale(EntityTransform& other) {
		return other.getScale() * getScale();
	}

	void EntityTransform::updateTransform() {
		// Methods like slerp need updating here
	}

	void EntityTransform::calcWorldSpaceMatrix(EntityTransform& other) {
		glm::vec3& pos = getWorldSpacePosition(other); //m_Transform.getPosition(); //glm::vec3(0.0f);//
		glm::vec3& rotation = getWorldSpaceRotation(other); // m_Transform.getRotation(); //glm::vec3(0.0f);//
		glm::vec3& scale = getWorldSpaceScale(other); // m_Transform.getScale(); //glm::vec3(1.0f);// 

		m_WorldSpaceTransformMatrix = glm::mat4(1.0f);
		m_WorldSpaceTransformMatrix = glm::translate(m_WorldSpaceTransformMatrix, pos);
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_WorldSpaceTransformMatrix = glm::scale(m_WorldSpaceTransformMatrix, scale);
	}

}