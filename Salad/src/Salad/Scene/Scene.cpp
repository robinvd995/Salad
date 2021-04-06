#include "sldpch.h"
#include "Scene.h"

#include "Entity.h"
#include "Components.h"

#include "Salad/Renderer/Renderer.h"

namespace Salad {

	Scene::Scene() {}

	Scene::~Scene() {}

	Entity Scene::createEntity(const std::string& tag) { 
		Entity entity = { m_Registry.create(), this };
		entity.addComponent<TransformComponent>();
		entity.addComponent<TagComponent>(tag);
		entity.addComponent<RelationshipComponent>();
		return entity;
	}

	void Scene::removeEntity(Entity entity) {
		m_Registry.destroy(entity);
	}

	void Scene::onUpdateEditor(Timestep& ts, Camera& camera, glm::mat4& viewMatrix) {

		Salad::Renderer::beginScene({ camera.getProjection(), viewMatrix });

		// Calculates Transformation Matrices where marked dirty
		{
			// TODO:	Add a "Dirty" Component to the child entities in need of updating
			//			then group on TransformComponent,RelationshipComponent,Dirty
			//			and update only the correct entities. This will make it so the sorting,
			//			which is not beeing done atm, will not be as expensive as sorting all the static
			//			entities in the scene
			//			Sorting example:
			// 
			//				group.sort([this](const entt::entity lhd, const entt::entity rhd) {
			//					EntityTransform& clhs = m_Registry.get<TransformComponent>(lhd);
			//					EntityTransform& crhs = m_Registry.get<TransformComponent>(rhd);
			//					return clhs.m_HierarchyDepth < crhs.m_HierarchyDepth;
		    //				});
			//
			//	Source: https://skypjack.github.io/2019-08-20-ecs-baf-part-4-insights/

			auto group = m_Registry.group<RelationshipComponent>(entt::get<TransformComponent>);

			for(auto entity : group) {
				auto [tc, relation] = group.get<TransformComponent, RelationshipComponent>(entity);
				EntityTransform& entityTransform = tc.Transform;

				if (entityTransform.m_TransformDirty) {
					Entity parent = { relation.parentid, this };
					EntityTransform& relativeTransform = parent ? parent.getComponent<TransformComponent>() : m_RootTransform;
					entityTransform.calcWorldSpaceMatrix(relativeTransform);

					entityTransform.m_TransformDirty = false;

					for(entt::entity childid : relation.children) {
						Entity child = { childid, this };
						EntityTransform& childTransform = child.getComponent<TransformComponent>();
						childTransform.m_TransformDirty = true;
					}
				}
			}
		}

		// Rendering Meshes

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group) {
			auto [t, m] = group.get<TransformComponent, MeshComponent>(entity);

			m.MeshTexture->bind();
			Salad::Renderer::submit(m.MeshShader, m.MeshVertexArray, t.Transform.getWorldSpaceTransformationMatrix());
		}

		Salad::Renderer::endScene();
	}

	void Scene::onUpdate(Timestep& ts) {

		// Update Scripts
		{
			m_Registry.view<NativeScriptComponent>().each([=](auto entity, NativeScriptComponent& nsc) {

				if(!nsc.instance){
					nsc.instance = nsc.instantiateScript();
					nsc.instance->m_Entity = { entity, this };
					nsc.instance->onCreate();
				}

				nsc.instance->onUpdate(ts);
			});
		}

		// Calculates Transformation Matrices where marked dirty
		{
			auto view = m_Registry.view<TransformComponent>();
			for (auto entity : view) {
				auto& tc = view.get<TransformComponent>(entity);
				tc.Transform.updateTransform();
				// BEFORE ENTITY_TRANSFORM: tc.Transform.calculateTransformationMatrix();
			}
		}

		Camera* mainCam = nullptr;
		// BEFORE ENTITY_TRANSFORM: Transform* mainCamTransform = nullptr;
		EntityTransform* mainCamTransform = nullptr;
		{
			auto group = m_Registry.group<CameraComponent>(entt::get<TransformComponent>);
			for(auto entity : group) {
				auto [camTransform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if(camera.Camera.isPrimaryCamera()) {
					mainCam = &camera.Camera;
					mainCamTransform = &camTransform.Transform;
					break;
				}
			}
		}

		if(mainCam) {
		
			// BEFORE ENTITY_TRANSFORM: Salad::Renderer::beginScene({ mainCam->getProjection(), glm::inverse(mainCamTransform->getMatrix()) });
			Salad::Renderer::beginScene({ mainCam->getProjection(), glm::inverse(mainCamTransform->getWorldSpaceTransformationMatrix()) });

			// Rendering Meshes

			auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
			for (auto entity : group) {
				auto [t, m] = group.get<TransformComponent, MeshComponent>(entity);

				m.MeshTexture->bind();
				// BEFORE ENTITY_TRANSFORM: Salad::Renderer::submit(m.MeshShader, m.MeshVertexArray, t.Transform.getMatrix());
				Salad::Renderer::submit(m.MeshShader, m.MeshVertexArray, t.Transform.getWorldSpaceTransformationMatrix());
			}
		
			Salad::Renderer::endScene();
		}
		

	}

	Entity Scene::getFirstTagged(const std::string& tag) {
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view) {
			auto& entityTag = view.get<TagComponent>(entity).Tag;
			if (entityTag == tag) return { entity, this };
		}
	}

	// Can do this but include components in scene header file

	/*template<typename T>
	bool Scene::entityHasComponent(Entity& entity) {
		return m_Registry.has<T>(entity);
	}

	template<typename T, typename... Args>
	T& Scene::entityAddComponent(Entity& entity, Args&&... args) {
		SLD_CORE_ASSERT(!entityHasComponent<T>(entity), "Entity already has component!");
		return m_Registry.emplace<T>(entity, std::forward<Args>(args)...);
	}

	template<typename T>
	T& Scene::entityGetComponent(Entity& entity) {
		SLD_CORE_ASSERT(entityHasComponent<T>(entity), "Entity does not have component!");
		return m_Registry.get<T>(entity);
	}

	template<typename T>
	void Scene::entityRemoveComponent(Entity& entity) {
		SLD_CORE_ASSERT(entityHasComponent<T>(entity), "Entity does not have component!");
		m_Registry.remove<T>(entity);
	}*/

	// ----- Entity Relationship Start -----

	/*void Scene::setEntityParentChildRelation(Entity& parent, Entity& child) {
		RelationshipComponent& parentComponent = parent.getComponent<RelationshipComponent>();
		RelationshipComponent& childComponent = child.getComponent<RelationshipComponent>();

		//parentComponent.data.children.push_back(child);
		childComponent.parent = parent;
		childComponent.depth = 0;

		uint32_t depth = parentComponent.depth + 1;
		childComponent.depth = depth;

	}*/

	void Scene::setEntityParent(Entity& parent, Entity& child) {
		//EntityTransform& parentTransform = parent.getComponent<TransformComponent>();
		//EntityTransform& childTransform = child.getComponent<TransformComponent>();

		RelationshipComponent& parentComponent = parent.getComponent<RelationshipComponent>();
		RelationshipComponent& childComponent = child.getComponent<RelationshipComponent>();

		parentComponent.children.push_back(child);
		childComponent.parentid = parent;
		childComponent.hierarchyDepth = parentComponent.hierarchyDepth + 1;

		//childTransform.setParent(parent);
		//parentTransform.addChild(child);
	}

	/*void Scene::addEntityChild(Entity& parent, Entity& child) {

	}*/

	// ----- Entity Relationship End -----

}