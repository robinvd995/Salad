#pragma once

#include "Salad/Core/Transform.h"

namespace Salad {

	/*
	Wrapper class for Transform
	Entity Hierarchy stuff should also be handled here
	NOTE TO SELF: Never expose Transform to keep the Transform class a mathematical bloat free class only
	*/
	class EntityTransform {

	public:
		EntityTransform() : 
			m_WorldSpaceTransformMatrix(glm::scale(m_WorldSpaceTransformMatrix, glm::vec3(1.0f)))
		{}

		//EntityTransform(const EntityTransform&) = default;
		virtual ~EntityTransform() = default;

		// ----- Transform Exposure Start -----

		void setPosition(float x, float y, float z) { m_Transform.setPosition(x, y, z); m_TransformDirty = true; }
		void setPosition(glm::vec3& position) { setPosition(position.x, position.y, position.z); }
		void translate(float x, float y, float z) { m_Transform.translate(x, y, z); m_TransformDirty = true; }

		//void setRotation(float x, float y, float z) { m_Transform.setRotation(x, y, z); m_TransformDirty = true; }
		//void setRotation(glm::vec3& rotation) { setRotation(rotation.x, rotation.y, rotation.z); }
		//void rotate(float x, float y, float z) { m_Transform.rotate(x, y, z); m_TransformDirty = true; }
		//void rotate(glm::vec3 rotation) { rotate(rotation.x, rotation.y, rotation.z); }

		void setScale(float x, float y, float z) { m_Transform.setScale(x, y, z); m_TransformDirty = true; }
		void setScale(glm::vec3 scale) { setScale(scale.x, scale.y, scale.z); }
		void scale(float x, float y, float z) { m_Transform.scale(x, y, z); m_TransformDirty = true; }

		void set(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale) { m_Transform.setValues(position, rotation, scale); m_TransformDirty = true; }

		glm::vec3 getPosition() { return m_Transform.getPosition(); }
		glm::vec3 getRotation() { return m_Transform.getRotation(); }
		glm::vec3 getScale() { return m_Transform.getScale(); }

		// ----- Transform Exposure End -----

		void updateTransform();
		const glm::mat4& getWorldSpaceTransformationMatrix() const { return m_WorldSpaceTransformMatrix; }

		// ----- Hierarchy methods start -----
		glm::vec3 getWorldSpacePosition(EntityTransform& other);
		glm::vec3 getWorldSpaceRotation(EntityTransform& other);
		glm::vec3 getWorldSpaceScale(EntityTransform& other);

		//void setParent(Entity parent);
		//void addChild(Entity child);

		//void onParentTransformChanged();
		// ----- Hierarchy methods end -----
	private:
		//void notifyChildrenOnChange();
		void calcWorldSpaceMatrix(EntityTransform& other);

	private:
		Transform m_Transform;

		bool m_TransformDirty = true;

		glm::mat4 m_WorldSpaceTransformMatrix{ 1.0f };

		friend class Scene;

	};

}