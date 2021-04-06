#pragma once

#include <string>
#include <vector>
#include <map>

namespace Salad {

	enum class PropertyType {
		Unknown = 0,
		Float = 1,
		Float2 = 2,
		Float3 = 3,
		Float4 = 4,
		Int = 5,
		Int2 = 6,
		Int3 = 7,
		Int4 = 8,
		ColorRGB = 9,
		ColorRGBA = 10,
		Mat2x2 = 11,
		Mat3x3 = 12,
		Mat4x4 = 13,
		Flag = 14,
		Seperator = 15,
		Tag = 16,
		ID = 17,
		PopID = 18
	};

	struct Property {

		PropertyType type;
		std::string name;
		void* data;

		Property(PropertyType p_type, const std::string& p_name, void* p_data) :
			type(p_type),
			name(p_name),
			data(p_data) 		
		{}

	};

	class EditorSettings {

	public:
		EditorSettings() = default;
		virtual ~EditorSettings() = default;

		void createGroup(const std::string& group) {
			m_PropertyMap.insert({ group, std::vector<Property>() });
		}

		void pushProperty(const std::string& group, PropertyType type, const std::string& propertyName, void* data) {
			auto iterator = m_PropertyMap.find(group);
			if (iterator == m_PropertyMap.end()) return;
			iterator->second.push_back({ type, propertyName, data });
		}

	private:
		std::map<std::string, std::vector<Property>> m_PropertyMap;

		friend class EditorSettingsWindow;
	};

	class EditorSettingsWindow {

	public:

		EditorSettingsWindow() = default;
		virtual ~EditorSettingsWindow() = default;

		bool& showWindow() { return m_ShowWindow; }

		void createGroup(const std::string& group) { m_Settings.createGroup(group); }
		void pushProperty(const std::string& group, PropertyType type, const std::string& name, void* data) { m_Settings.pushProperty(group, type, name, data); };
		void pushSeperator(const std::string& group) { m_Settings.pushProperty(group, PropertyType::Seperator, "", nullptr); }
		void pushTag(const std::string& group, const std::string& tag) { m_Settings.pushProperty(group, PropertyType::Tag, tag, nullptr); }
		void pushID(const std::string& group, const std::string& id) { m_Settings.pushProperty(group, PropertyType::ID, "", nullptr); }
		void popID(const std::string& group) { m_Settings.pushProperty(group, PropertyType::PopID, "", nullptr); }

		void onImGuiRender();

	private:
		EditorSettings m_Settings;
		bool m_ShowWindow = false;
		std::string m_SelectedGroup = "";
	};

}
