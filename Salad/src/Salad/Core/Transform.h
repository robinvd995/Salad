#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include <glm/gtc/quaternion.hpp>

namespace Salad {

	class Transform {

	public:
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(glm::mat4 transformMatrix) : 
			m_TransformMatrix(transformMatrix)
		{}

		void setPosition(float x, float y, float z);
		void translate(float x, float y, float z);

		/*void setRotation(float x, float y, float z);
		void rotate(float x, float y, float z);*/

		void setScale(float x, float y, float z);
		void scale(float x, float y, float z);

		void setValues(glm::vec3& position, glm::quat& rotation, glm::vec3& scale);

		glm::vec3& getPosition() { return m_Position; }
		glm::vec3& getEulerRotation() { return glm::eulerAngles(m_Orientation); }
		glm::quat& getOrientation() { return m_Orientation; }
		glm::vec3& getScale() { return m_Scale; }

		const glm::mat4& getMatrix() const { return m_TransformMatrix; }
		glm::mat4 getMatrixCpy() { return m_TransformMatrix; }

		// ---- Entity Hierarchy Start -----
		//void setRelativeMatrix(glm::mat4& matrix) { m_RelativeMatrix = matrix; calculateWorldSpaceMatrix(); }
		//glm::mat4 getRelativeMatrix() { return m_RelativeMatrix; }

		//glm::mat4& getWorldSpaceMatrix() { return m_WorldSpaceMatrix; }
		// ---- Entity Hierarchy End -----

		void calculateTransformationMatrix();

		// ---- Entity Hierarchy Start -----
		//void calculateWorldSpaceMatrix();
		// ---- Entity Hierarchy End -----

	private:
		glm::vec3 m_Position { 0.0f };
		//glm::vec3 m_Rotation { 0.0f };
		glm::quat m_Orientation{};
		glm::vec3 m_Scale    { 1.0f };

		glm::mat4 m_TransformMatrix{ 1.0f };

		// ---- Entity Hierarchy Start -----
		//glm::mat4 m_RelativeMatrix = glm::scale(glm::mat4{ 1.0f }, glm::vec3{ 1.0f });
		//glm::mat4 m_WorldSpaceMatrix{ 1.0f };
		// ---- Entity Hierarchy End -----
	};

}
