#pragma once

#include "Salad/Scene/Entity.h"

#include "Assets/ShaderAsset.hpp"
#include "Assets/TextureAsset.hpp"
#include "Assets/ModelAsset.hpp"
#include "Assets/MaterialAsset.hpp"

namespace Salad {

	enum class EditorSelectionContextType {
		None = 0,
		Entity = 1,
		EntityGroup = 2,
		Shader = 3,
		Texture = 4,
		Model = 5,
		Material = 6
	};

	class EntitySelectionContext { 

	public:
		EntitySelectionContext() = default;
		EntitySelectionContext(Entity entity) : m_SelectedEntity(entity) {}
		~EntitySelectionContext() = default;

		// Selection Context Mandatory Functions
		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Entity; }
		static void deallocate(void* ptr) { delete ((EntitySelectionContext*) ptr); }

		// EntitySelectionContext specific functions
		void setSelectedEntity(Entity entity) { m_SelectedEntity = entity; }
		Entity getSelectedEntity() { return m_SelectedEntity; }
		bool isSelectionValid() { return m_SelectedEntity.isValid(); }
		bool isEntitySelected(Entity entity) { return m_SelectedEntity == entity; }

	private:
		Entity m_SelectedEntity;

	};

	class ShaderSelectionContext {
	
	public:
		ShaderSelectionContext() = default;
		ShaderSelectionContext(Asset::ShaderAsset& shader) : m_EditorShader(shader) {}
		~ShaderSelectionContext() = default;

		// Selection Context Mandatory Functions
		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Shader; }
		static void deallocate(void* ptr) { delete ((ShaderSelectionContext*)ptr); }

		// ShaderSelectionContext specific functions
		Asset::ShaderAsset& getEditorShader() { return m_EditorShader; }

	private:
		Asset::ShaderAsset m_EditorShader;
	};

	class TextureSelectionContext {
	
	public:
		TextureSelectionContext() = delete;
		TextureSelectionContext(Asset::TextureAsset& texture) : m_Texture(texture) {};
		~TextureSelectionContext() = default;

		// Selection Context Mandatory Functions
		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Texture; }
		static void deallocate(void* ptr) { delete ((TextureSelectionContext*)ptr); }

		// TextureSelectionContext specific functions
		Asset::TextureAsset& getTexture() { return m_Texture; }

	private:
		Asset::TextureAsset m_Texture;
	};

	class ModelSelectionContext {
	
	public:
		ModelSelectionContext() = delete;
		ModelSelectionContext(Ref<Asset::ModelAsset> model) : m_Model(model) {}
		~ModelSelectionContext() = default;

		// Selection Context Mandatory Functions
		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Model; }
		static void deallocate(void* ptr) { delete ((ModelSelectionContext*)ptr); }

		Ref<Asset::ModelAsset> getModel() { return m_Model; }

	private:
		Ref<Asset::ModelAsset> m_Model;
	};

	class MaterialSelectionContext {
	
	public:
		MaterialSelectionContext() = delete;
		MaterialSelectionContext(Ref<Asset::MaterialAsset> material) : m_Material(material) {}
		~MaterialSelectionContext() = default;

		// Selection Context Mandatory Functions
		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Material; }
		static void deallocate(void* ptr) { delete ((ModelSelectionContext*)ptr); }

		Ref<Asset::MaterialAsset> getMaterial() { return m_Material; }

	private:
		Ref<Asset::MaterialAsset> m_Material;
	};

	class EditorSelectionContext {

	public:
		template<typename T, typename... Args>
		static void setSelectionContext(Args&&... args) { s_Instance->setSelectionContextImpl<T>(std::forward<Args>(args)...); }
		
		template<typename T>
		static T* getSelectionContext() { return s_Instance->getSelectionContextImpl<T>(); };

		static EditorSelectionContextType getSelectionContextType() { return s_Instance->m_ContextType; }
		static bool isSelectionContextType(EditorSelectionContextType type) { return s_Instance->m_ContextType == type; }

		static void setSelectionContextNone() { s_Instance->setSelectionContextNoneImpl(); }

	private:
		EditorSelectionContext() = default;
		~EditorSelectionContext() { setSelectionContextNoneImpl(); }

		void clearCurrentContext() {
			m_DeallocFunction(m_SelectionContext);
		}

		template<typename T, typename... Args>
		void setSelectionContextImpl(Args&&... args) {
			if (m_SelectionContext != nullptr) clearCurrentContext();

			m_ContextType = T::getSelectionType();
			m_DeallocFunction = T::deallocate;
			T* selectionContext = new T(args...);
			m_SelectionContext = (void*)selectionContext;
		}
		
		template<typename T>
		T* getSelectionContextImpl() {
			EditorSelectionContextType type = T::getSelectionType();
			if (m_ContextType == type) return (T*)m_SelectionContext;
			else return nullptr;
		}

		void setSelectionContextNoneImpl() {
			if (m_SelectionContext != nullptr) clearCurrentContext();
			m_ContextType = EditorSelectionContextType::None;
			m_SelectionContext = nullptr;
		}

	private:
		EditorSelectionContextType m_ContextType;
		void* m_SelectionContext;
		std::function<void(void*)> m_DeallocFunction;

		inline static EditorSelectionContext* s_Instance;

		friend class EditorLayer; // TODO: Change to EditorScene when it is implemented
	};

}
