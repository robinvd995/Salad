#include "sldpch.h"
#include "EntityTransform.h"

namespace Salad {

	glm::vec3 EntityTransform::getWorldSpacePosition(EntityTransform& other) {
		return other.getPosition() + getPosition();
	}

	glm::quat EntityTransform::getWorldSpaceRotation(EntityTransform& other) {
		return other.getOrientation() + getOrientation();
	}

	glm::vec3 EntityTransform::getWorldSpaceScale(EntityTransform& other) {
		return other.getScale() * getScale();
	}

	void EntityTransform::updateTransform() {
		// Methods like slerp need updating here
	}

	void EntityTransform::calcWorldSpaceMatrix(EntityTransform& other) {
		glm::vec3& pos = getPosition() + other.getPosition();//getWorldSpacePosition(other); //m_Transform.getPosition(); //glm::vec3(0.0f);//
		glm::vec3& rotation = getEulerRotation() + other.getEulerRotation();//getWorldSpaceRotation(other); // m_Transform.getRotation(); //glm::vec3(0.0f);//
		glm::vec3& scale = getScale() * other.getScale();//getWorldSpaceScale(other); // m_Transform.getScale(); //glm::vec3(1.0f);// 

		m_WorldSpaceTransformMatrix = glm::mat4(1.0f);
		m_WorldSpaceTransformMatrix = glm::translate(m_WorldSpaceTransformMatrix, pos);
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		m_WorldSpaceTransformMatrix = glm::rotate(m_WorldSpaceTransformMatrix, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		m_WorldSpaceTransformMatrix = glm::scale(m_WorldSpaceTransformMatrix, scale);

		const glm::mat4& otherTransform = other.getWorldSpaceTransformationMatrix();
	}

}