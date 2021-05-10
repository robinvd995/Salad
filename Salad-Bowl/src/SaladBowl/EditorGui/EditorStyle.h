#pragma once

#include "Salad/Renderer/Texture.h"

#include "imgui/imgui.h"

#include <string>

namespace Salad::EditorGui {

	enum class EditorUIStyle {
		Default = 0,
		VisualStudio = 1,
		DarkCharcoal = 2
	};

	class EditorStyle {
	
	public:
		EditorStyle() = default;
		~EditorStyle() = default;
	
		inline static void setEditorStyle(EditorUIStyle styletype) { s_Instance->setEditorStyleImpl(styletype); }
		inline static void loadEditorIcons(const std::string& filepath) { s_Instance->loadEditorIconsImpl(filepath); }
		inline static void loadFileIcons(const std::string& filepath) { s_Instance->loadFileIconsImpl(filepath); }
		inline static Ref<Texture2D> getEditorIcons() { return s_Instance->m_EditorIcons; }
		inline static Ref<Texture2D> getFileIcons() { return s_Instance->m_FileIcons; }

	private:
		void setEditorStyleImpl(EditorUIStyle styletype);
		void loadEditorIconsImpl(const std::string& filepath);
		void loadFileIconsImpl(const std::string& filepath);

	private:
		inline static EditorStyle* s_Instance;
		friend class EditorUI;

	private:
		Ref<Texture2D> m_EditorIcons;
		Ref<Texture2D> m_FileIcons;
		ImVec4 m_StatusBarBgColor;
	};
}