#ifdef OLD_ENTITY_SYSTEM
#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Core/Transform.h"
#include "Salad/Core/Timestep.h"
#include "Salad/Renderer/Sprite.h"

#include <glm/glm.hpp>

namespace Salad {

	enum class ComponentType {
		/* NO TYPE */         None = 0,
		/* Essential Types*/  Transform,
		/* Render Types*/     SpriteRenderer,
		/* Controller Types*/ PlayerController,
		/* Behaviour */       EntityBehaviour
	};

	class Entity;

	class EntityComponent {

	public:
		virtual void onComponentAttached() {};
		virtual void onComponentDetached() {};
		virtual void onComponentUpdate(Timestep ts) {};

		virtual ComponentType getComponentType() const = 0;

		virtual bool allowDuplicate() { return false; }

		void setParent(Ref<Entity>& entity) { m_Entity = entity; }

	protected:
		WeakRef<Entity> m_Entity;
	};

	class Entity {

	public:
		Entity();
		~Entity();
		void onUpdate(Timestep ts);

		template<typename T>
		Ref<T> getComponent();

		static void attachComponent(Ref<Entity>& entity, Ref<EntityComponent> component);

	private:
		void attachComponent(Ref<EntityComponent> component);
		void detachComponent(ComponentType componentType);
	private:
		//Make list, it allows for duplicate components like scripts
		std::unordered_map<ComponentType, Ref<EntityComponent>> m_EntityComponentMap;
	};

#define EC_CLASS_TYPE(type) static ComponentType getStaticType() { return ComponentType::type; }\
virtual ComponentType getComponentType() const override { return getStaticType(); }

	class EntityComponentTransform : public EntityComponent {

	public:
		EntityComponentTransform() :
			m_Transform(createRef<Transform>())
		{}

		EntityComponentTransform(const EntityComponentTransform& other) = delete;

		~EntityComponentTransform() {}

		/*inline glm::vec3& getPosition() { return m_Position; }
		inline glm::vec3& getRotation() { return m_Rotation; }
		inline glm::vec3& getScale() { return m_Scale; };

		inline glm::vec3 cpyPosition() const { return m_Position; }
		inline glm::vec3 cpyRotation() const { return m_Rotation; }
		inline glm::vec3 cpyScale() const { return m_Scale; };

		void setPosition(float x, float y, float z);
		void setPosition(glm::vec3& position);
		void setRotation(float x, float y, float z);
		void setRotation(glm::vec3& rotation);
		void setScale(float x, float y, float z);
		void setScale(glm::vec3& scale);

		void translate(float x, float y);
		void translate(const glm::vec2& delta);
		void translate(float x, float y, float z);
		void translate(const glm::vec3& delta);*/

		inline Ref<Transform> getTransform() { return m_Transform; }

		EC_CLASS_TYPE(Transform)

	private:
		Ref<Transform> m_Transform;
	};

	struct SpriteRenderInformation {
		Ref<Texture2D> spriteTexture;
		float posU = 0.0f, posV = 1.0f, sizeU = 0.0f, sizeV = 1.0f;

		SpriteRenderInformation(Ref<Texture2D> p_spriteTexture, float u, float v, float w, float h) :
			spriteTexture(p_spriteTexture),
			posU(u),
			posV(v),
			sizeU(w),
			sizeV(h) 
		{}
	};

	// TODO Place in seperate file, current restriction is the template function getComponent
	class EntityComponentSpriteRenderer : public EntityComponent {

	public:
		EntityComponentSpriteRenderer() {}
		EntityComponentSpriteRenderer(const EntityComponentSpriteRenderer& ecsr) = delete;
		//EntityComponentSpriteRenderer(Ref<Sprite>& sprite, float frameTime, int frameCount);

		void setSprite(const std::string spriteId);
		void setSprite(Ref<Sprite> sprite);
		SpriteRenderInformation getSpriteRenderInformation();

		virtual void onComponentUpdate(Timestep ts) override;

		Ref<Sprite>& getSprite() { return m_Sprite; }

		EC_CLASS_TYPE(SpriteRenderer)
		
	private:
		Ref<Sprite> m_Sprite;
		SpriteAnimator m_Animator;
	};

	class EntityComponentPlayerController : public EntityComponent {

	public:
		EntityComponentPlayerController() {}
		EntityComponentPlayerController(const EntityComponentPlayerController& other) = delete;

		virtual void onComponentAttached() override;

		virtual void onComponentUpdate(Timestep ts) override;

		EC_CLASS_TYPE(PlayerController)

	private:
		Ref<EntityComponentTransform> m_Transform;
		Ref<EntityComponentSpriteRenderer> m_SpriteRenderer;
		float m_MoveSpeed = 2.0f;

		uint32_t m_Direction = 0;
		bool m_Moved = false;

		std::string m_CurSprite = "";
	};

	class EntityBehaviour;

	class EntityComponentBehaviour : public EntityComponent {
	
	public:
		EntityComponentBehaviour(Ref<EntityBehaviour> behaviour) :
			m_Behaviour(behaviour)
		{}

		~EntityComponentBehaviour() {}

		virtual void onComponentAttached() override;
		virtual void onComponentUpdate(Timestep ts) override;

		EC_CLASS_TYPE(EntityBehaviour)

	private:
		Ref<EntityBehaviour> m_Behaviour;
	};

	class EntityBehaviour {
	
	public:
		virtual void start() = 0;
		virtual void update(Timestep ts) = 0;
		virtual void end() = 0;

	protected:
		Ref<Transform> m_Transform;

	private:
		void setTransform(Ref<Transform> transform) { m_Transform = transform; }
		friend class EntityComponentBehaviour;

	private:

	};

	// TODO find a way to declare this in another class and prototype the class
	template Ref<EntityComponentTransform> Entity::getComponent();
	template Ref<EntityComponentSpriteRenderer> Entity::getComponent();
	template Ref<EntityComponentPlayerController> Entity::getComponent();
	template Ref<EntityComponentBehaviour> Entity::getComponent();
}

#endif // OLD_ENTITY_SYSTEM