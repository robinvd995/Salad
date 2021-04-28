#pragma once

#include "Salad/Core/Timestep.h"

#include "Salad/Renderer/Camera.h"
#include "EntityTransform.h"

#include "entt.hpp"

#include <glm/glm.hpp>

// -- TEMP -- TODO: remove this and the friend class for scene hierarchy panel into EditorScene
namespace Salad::EditorGui {
	class SceneHierarchyPanel;     // Prototyping SceneHierarchyPanel class used in Salad-Bowl (Editor)
}

namespace Salad {

	class Entity;                  // Prototyping Entity class to prevent circle includes

	class Scene {
	
	public:
		Scene();
		~Scene();

		void onUpdateEditor(Timestep& ts, Camera& camera, glm::mat4& transform, Entity& selectedEntity);
		void onUpdate(Timestep& ts);

		Entity createEntity(const std::string& tag = std::string("Unnamed Entity"));
		void removeEntity(Entity entity);

		Entity getFirstTagged(const std::string& tag);

		/*template<typename T>
		bool entityHasComponent(Entity& entity);

		template<typename T, typename... Args>
		T& entityAddComponent(Entity& entity, Args&&... args);

		template<typename T>
		T& entityGetComponent(Entity& entity);

		template<typename T>
		void entityRemoveComponent(Entity& entity);*/

		// ----- Entity Relationship Start -----

		void setEntityParent(Entity& parent, Entity& child);

		//void setEntityParentChildRelation(Entity& parent, Entity& child);

		// ----- Entity Relationship End -----

	private:
		entt::registry m_Registry;

		EntityTransform m_RootTransform;

		friend class Entity;
		friend class EditorGui::SceneHierarchyPanel;
		friend class SceneSerializer;
	};

}
