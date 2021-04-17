#pragma once

#include "Salad/Scene/Entity.h"
#include "EditorShader.h"

namespace Salad {

	enum class EditorSelectionContextType {
		None = 0,
		Entity = 1,
		EntityGroup = 2,
		Shader = 3
	};

	class EntitySelectionContext { 

	public:
		EntitySelectionContext() = default;
		EntitySelectionContext(Entity entity) : m_SelectedEntity(entity) {}
		~EntitySelectionContext() = default;

		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Entity; }

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
		ShaderSelectionContext(EditorShader& shader) : m_EditorShader(shader) {}
		~ShaderSelectionContext() = default;

		static EditorSelectionContextType getSelectionType() { return EditorSelectionContextType::Shader; }

		EditorShader& getEditorShader() { return m_EditorShader; }

	private:
		EditorShader m_EditorShader;
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

		template<typename T, typename... Args>
		void setSelectionContextImpl(Args&&... args) {
			if (m_SelectionContext != nullptr) delete m_SelectionContext;

			m_ContextType = T::getSelectionType();
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
			m_ContextType = EditorSelectionContextType::None;
			if (m_SelectionContext != nullptr) delete m_SelectionContext;
			m_SelectionContext = nullptr;
		}

	private:
		EditorSelectionContextType m_ContextType;
		void* m_SelectionContext;

		inline static EditorSelectionContext* s_Instance;

		friend class EditorLayer; // TODO: Change to EditorScene when it is implemented
	};

}
