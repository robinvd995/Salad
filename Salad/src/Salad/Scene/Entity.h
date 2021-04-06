#pragma once

#include "Salad/Core/Log.h"
#include "Scene.h"

#include "Salad/Core/Transform.h"

#include "entt.hpp"

namespace Salad {

	class Entity {
	
	public:
		Entity() = default;
		Entity(uint32_t handle, Scene* scene) : m_EntityId((entt::entity)handle), m_Scene(scene) {}
		Entity(entt::entity handle, Scene* scene) : m_EntityId(handle), m_Scene(scene) {}
		Entity(const Entity& other) = default;

		template<typename T>
		bool hasComponent() {
			return m_Scene->m_Registry.has<T>(m_EntityId);
		}

		template<typename T, typename... Args>
		T& addComponent(Args&&... args) {
			SLD_CORE_ASSERT(!hasComponent<T>(), "Entity already has component!");
			return m_Scene->m_Registry.emplace<T>(m_EntityId, std::forward<Args>(args)...);
		}

		template<typename T>
		T& getComponent(){
			SLD_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			return m_Scene->m_Registry.get<T>(m_EntityId);
		}

		template<typename T>
		void removeComponent() {
			SLD_CORE_ASSERT(hasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityId);
		}

		bool isValid() { return m_EntityId != entt::null; }

		operator bool() const { return m_EntityId != entt::null; }
		operator uint32_t() const { return (uint32_t)m_EntityId; }
		operator entt::entity() const { return m_EntityId; }

		// ----- Entity Relationship Start -----

		void addChild(Entity child) { SLD_CORE_ERROR("Adding a child is currently not supported!");/*m_Scene->setEntityParentChildRelation(*this, child);*/ }

		void setParent(Entity parent) { m_Scene->setEntityParent(parent, *this); };

		// ----- Entity Relationship End -----

		bool operator ==(const Entity& other) const { return m_EntityId == other.m_EntityId && m_Scene == other.m_Scene; }
		bool operator !=(const Entity& other) const { return (*this) == other; }


	private:
		entt::entity m_EntityId{ entt::null };
		//TODO: Change to weakref
		Scene* m_Scene = nullptr;
	};
}
