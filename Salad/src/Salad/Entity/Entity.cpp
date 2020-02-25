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
	Ref<T> Entity::getComponent() {
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

	void EntityComponentTransform::translate(float x, float y) {
		m_Position.x += x;
		m_Position.y += y;
	}

	void EntityComponentTransform::translate(const glm::vec2& delta) {
		m_Position.x += delta.x;
		m_Position.y += delta.y;
	}

	void EntityComponentTransform::translate(float x, float y, float z) {
		m_Position.x += x;
		m_Position.y += y;
		m_Position.z += z;
	}

	void EntityComponentTransform::translate(const glm::vec3& delta) {
		m_Position.x += delta.x;
		m_Position.y += delta.y;
		m_Position.z += delta.z;
	}

	// -------- Transform Component End --------

	// -------- Sprite Render Component --------

	void EntityComponentSpriteRenderer::setSprite(const std::string spriteId) {
		m_Sprite = SpriteLibrary::get().getSprite(spriteId);
	}

	void EntityComponentSpriteRenderer::setSprite(Ref<Sprite> sprite) {
		m_Sprite = sprite;
	}

	SpriteRenderInformation EntityComponentSpriteRenderer::getSpriteRenderInformation() {

		float resolution = 1.0f / m_Sprite->getSpriteTexture()->getWidth();

		float spriteWidth = (float)m_Sprite->getSpriteWidth() * resolution;
		float spriteHeight = (float)m_Sprite->getSpriteHeight() * resolution;

		uint32_t spriteIndexX = m_Animator.getCurrentFrame() % m_Sprite->getSpritesPerRow();
		uint32_t spriteIndexY = m_Animator.getCurrentFrame() / m_Sprite->getSpritesPerRow();

		float spriteTexCoordX = (m_Sprite->getTexCoordX() + (spriteIndexX * m_Sprite->getSpriteWidth())) * resolution;
		float spriteTexCoordY = (m_Sprite->getTexCoordY() + (spriteIndexY * m_Sprite->getSpriteHeight())) * resolution;

		return SpriteRenderInformation(m_Sprite->getSpriteTexture(), spriteTexCoordX, spriteTexCoordY, m_Sprite->getSpriteWidth() * resolution, m_Sprite->getSpriteHeight() * resolution);
	}

	void EntityComponentSpriteRenderer::onComponentUpdate(Timestep ts) {
		m_Animator.updateAnimator(ts, m_Sprite->getFrameTime(), m_Sprite->getFrameCount());
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

		glm::vec2 moveDirection(0.0f);

		if (Input::isKeyPressed(SLD_KEY_A)) {
			moveDirection.x -= 1;
		}

		if (Input::isKeyPressed(SLD_KEY_D)) {
			moveDirection.x += 1;
		}

		if (Input::isKeyPressed(SLD_KEY_W)) {
			moveDirection.y -= 1;
		}

		if (Input::isKeyPressed(SLD_KEY_S)) {
			moveDirection.y += 1;
		}

		bool moving = false;
		if(moveDirection.x != 0.0f || moveDirection.y != 0.0f) {
			moving = true;
		}
		if (moving) {
			glm::vec2 normalizedVector = glm::normalize(moveDirection);
			m_Transform->translate(normalizedVector * speed);
		}
		
		if (moveDirection.y > 0)
			m_Direction = 0;
		else if (moveDirection.y < 0) 
			m_Direction = 1;
		else if (moveDirection.x < 0)
			m_Direction = 2;
		else if (moveDirection.x > 0)
			m_Direction = 3;

		std::string spriteId = "";
		if (m_Direction == 0 && moving)
			spriteId = "player_move_up";
		else if (m_Direction == 0 && !moving)
			spriteId = "player_stand_up";
		else if (m_Direction == 1 && moving)
			spriteId = "player_move_down";
		else if (m_Direction == 1 && !moving)
			spriteId = "player_stand_down";
		else if (m_Direction == 2 && moving)
			spriteId = "player_move_right";
		else if (m_Direction == 2 && !moving)
			spriteId = "player_stand_right";
		else if (m_Direction == 3 && moving)
			spriteId = "player_move_left";
		else if (m_Direction == 3 && !moving)
			spriteId = "player_stand_left";

		if(spriteId != m_CurSprite) {
			m_Entity.lock()->getComponent<EntityComponentSpriteRenderer>()->setSprite(spriteId);
			m_CurSprite = spriteId;
		}

		m_Moved = moving;
	}

	// -------- Player Controller End --------
}
