#pragma once

#include <string>
#include <vector>
#include <map>

#include "SaladBowl/EditorSettings.hpp"

namespace Salad::EditorGui {

	class EditorSettingsWindow {

	public:

		EditorSettingsWindow() = default;
		virtual ~EditorSettingsWindow() = default;

		bool& showWindow() { return m_ShowWindow; }

		// void createGroup(const std::string& group) { m_Settings.createGroup(group); }
		// void pushProperty(const std::string& group, PropertyType type, const std::string& name, void* data) { m_Settings.pushProperty(group, type, name, data); };
		// void pushSeperator(const std::string& group) { m_Settings.pushProperty(group, PropertyType::Seperator, "", nullptr); }
		// void pushTag(const std::string& group, const std::string& tag) { m_Settings.pushProperty(group, PropertyType::Tag, tag, nullptr); }
		// void pushID(const std::string& group, const std::string& id) { m_Settings.pushProperty(group, PropertyType::ID, "", nullptr); }
		// void popID(const std::string& group) { m_Settings.pushProperty(group, PropertyType::PopID, "", nullptr); }

		void onImGuiRender();

	private:
		//EditorSettings m_Settings;
		bool m_ShowWindow = false;
		bool m_PreviousShowWindow = false;
		std::string m_SelectedGroup = "";

		friend class EditorUI;
	};

}
