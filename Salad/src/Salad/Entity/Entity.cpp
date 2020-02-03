#include "sldpch.h"
#include "Entity.h"

#include "Salad/Core/Input.h"
#include "Salad/Core/KeyCodes.h"

#include <type_traits>

namespace Salad {

#ifdef _DEBUG 1
	#define COMPONENT_DUPLICATE_CHECK(componentType, allowDuplicate)\
	std::unordered_map<ComponentType, Ref<EntityComponent>>::iterator it = m_EntityComponentMap.find(componentType);\
	std::unordered_map<ComponentType, Ref<EntityComponent>>::iterator end = m_EntityComponentMap.end();\
	bool assrt = it == end || allowDuplicate;\
	SLD_CORE_ASSERT(assrt, "Trying to add duplicate component!")
#else
	#define COMPONENT_DUPLICATE_CHECK(componentType, allowDuplicate)
#endif

	Entity::Entity() {
		//SLD_CORE_TRACE("Creating entity!");
	}

	Entity::~Entity() {
		//SLD_CORE_TRACE("Deleting entity!");
	}

	void Entity::attachComponent(Ref<EntityComponent> component) {
		COMPONENT_DUPLICATE_CHECK(component->getComponentType(), component->allowDuplicate());
		m_EntityComponentMap[component->getComponentType()] = component;
	}

	void Entity::detachComponent(ComponentType componentType) {
		auto it = m_EntityComponentMap.find(componentType);
		if(it != m_EntityComponentMap.end()){
			it->second->onComponentDetached();
			m_EntityComponentMap.erase(componentType);
		}
		else {
			SLD_WARN("Trying to remove component from entity with id '{0}', but does not exists!", componentType);
		}
	}

	void Entity::onUpdate(Timestep ts) {
		std::unordered_map<ComponentType, Ref<EntityComponent>>::iterator it;
		for(it = m_EntityComponentMap.begin(); it != m_EntityComponentMap.end(); it++){
			auto component = it->second;
			component->onComponentUpdate(ts);
		}
	}

	template<typename T>
	inline Ref<T> Entity::getComponent() {
		std::unordered_map<ComponentType, Ref<EntityComponent>>::iterator it;
		for (it = m_EntityComponentMap.begin(); it != m_EntityComponentMap.end(); it++) {
			auto component = it->second;
			if (component->getComponentType() == T::getStaticType()) {
				return std::static_pointer_cast<T>(component);
			}
		}
		SLD_CORE_WARN("Did not find component with id {0}!", T::getStaticType());
		return nullptr;
	}

	void Entity::attachComponent(Ref<Entity>& entity, Ref<EntityComponent> component) {
		entity->attachComponent(component);
		component->setParent(entity);
		component->onComponentAttached();
	}

	// -------- Entity Components Start --------

	// -------- Transform Component --------

	void EntityComponentTransform::setPosition(float x, float y, float z) {
		m_Position.x = x;
		m_Position.y = y;
		m_Position.z = z;
	}

	void EntityComponentTransform::setPosition(glm::vec3& position) {
		m_Position.x = position.x;
		m_Position.y = position.y;
		m_Position.z = position.z;
	}

	void EntityComponentTransform::setRotation(float x, float y, float z) {
		m_Rotation.x = x;
		m_Rotation.y = y;
		m_Rotation.z = z;
	}

	void EntityComponentTransform::setRotation(glm::vec3& rotation) {
		m_Rotation.x = rotation.x;
		m_Rotation.y = rotation.y;
		m_Rotation.z = rotation.z;
	}

	void EntityComponentTransform::setScale(float x, float y, float z) {
		m_Scale.x = x;
		m_Scale.y = y;
		m_Scale.z = z;
	}

	void EntityComponentTransform::setScale(glm::vec3& scale) {
		m_Scale.x = scale.x;
		m_Scale.y = scale.y;
		m_Scale.z = scale.z;
	}

	// -------- Transform Component End --------

	// -------- Sprite Render Component --------

	void EntityComponentSpriteRenderer::setSpriteRendererValues(Ref<Sprite> sprite, float frameTime, int frameCount) {
		m_Sprite = sprite;
		m_Animator.setAnimatorValues(frameTime, frameCount);
	}

	SpriteRenderInformation EntityComponentSpriteRenderer::getSpriteRenderInformation() {

		float texMapSizeH = 1.0f / (float)m_Sprite->getTextureMap()->getGridCountH();
		float texMapSizeV = 1.0f / (float)m_Sprite->getTextureMap()->getGridCountV();

		int hIndex = (m_Sprite->getSpriteIndex() + m_Animator.getCurFrame()) % m_Sprite->getTextureMap()->getGridCountH();
		int vIndex = (m_Sprite->getSpriteIndex() + m_Animator.getCurFrame()) / m_Sprite->getTextureMap()->getGridCountV();

		float hOffset = hIndex * 1.0f / m_Sprite->getTextureMap()->getGridCountH();
		float vOffset = vIndex * 1.0f / m_Sprite->getTextureMap()->getGridCountV();

		return SpriteRenderInformation(m_Sprite, hOffset, vOffset, texMapSizeH, texMapSizeV);
	}

	void EntityComponentSpriteRenderer::onComponentUpdate(Timestep ts) {
		m_Animator.updateAnimator(ts);
	}

	// -------- Sprite Render Component End --------

	// -------- Player Controller Component --------

	void EntityComponentPlayerController::onComponentAttached() {
		m_Transform = m_Entity.lock()->getComponent<EntityComponentTransform>();
		SLD_CORE_ASSERT(m_Transform, "Sprite Renderer is nullptr!");
		m_SpriteRenderer = m_Entity.lock()->getComponent<EntityComponentSpriteRenderer>();
		SLD_CORE_ASSERT(m_SpriteRenderer, "Transform is nullptr!");
	}

	void EntityComponentPlayerController::onComponentUpdate(Timestep ts) {

		glm::vec3& position = m_Transform->getPosition();
		float speed = m_MoveSpeed * ts;

		if (Input::isKeyPressed(SLD_KEY_A)) {
			position.x -= speed;
		}

		if (Input::isKeyPressed(SLD_KEY_D)) {
			position.x += speed;
		}

		if (Input::isKeyPressed(SLD_KEY_W)) {
			position.y += speed;
		}

		if (Input::isKeyPressed(SLD_KEY_S)) {
			position.y -= speed;
		}
	}

	// -------- Player Controller End --------
}
