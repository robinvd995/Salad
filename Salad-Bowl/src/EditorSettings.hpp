#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Core/Log.h"

#include "Util/FileUtils.hpp"

#include <string>
#include <map>
#include <vector>

#include <fstream>
#include <iomanip>
#include <json.hpp>

using json = nlohmann::json;

#define SLD_EDITOR_SETTINGS_FILEPATH "editor_settings.jcfg"

namespace Salad {

	enum class  EditorSettingsPropertyType {
	 	Unknown = 0, Float = 1, Int = 5, Flag = 14, String = 15
	};

	// enum class  EditorSettingsPropertyType {
	// 	Unknown = 0,
	// 	Float = 1, Float2 = 2, Float3 = 3, Float4 = 4,
	// 	Int = 5, Int2 = 6, Int3 = 7, Int4 = 8,
	// 	ColorRGB = 9, ColorRGBA = 10,
	// 	Mat2x2 = 11, Mat3x3 = 12, Mat4x4 = 13,
	// 	Flag = 14, String = 15
	// };

	struct EditorSettingsProperty {

		EditorSettingsPropertyType type;
		std::string name;
		void* data;

		EditorSettingsProperty(EditorSettingsPropertyType p_type, const std::string& p_name, void* p_data) :
			type(p_type),
			name(p_name),
			data(p_data)
		{}

	};

	struct EditorSettingsPropertyList {
		std::vector<EditorSettingsProperty> properties;

		EditorSettingsProperty* getPropertyFromName(std::string& name) {
			for(auto& prop : properties) {
				if (prop.name == name) return &prop;
			}
			return nullptr;
		}
	};

	struct EditorSettingsGroup { 
		std::string groupId;

		std::map<std::string,EditorSettingsPropertyList> subGroups;
		EditorSettingsPropertyList propertyList;
	};

	class EditorSettings {

	public:

		static void pushGroup(const std::string& group) { s_Instance->pushGroupImpl(group); }
		static void popGroup() { s_Instance->popGroupImpl(); }

		static void pushSubGroup(const std::string& subgroup) { s_Instance->pushSubGroupImpl(subgroup); }
		static void popSubGroup() { s_Instance->popSubGroupImpl(); }

		static void pushFloat(const std::string& propertyName, float* data) { s_Instance->pushPropertyImpl(EditorSettingsPropertyType::Float, propertyName, (void*)data); }
		static void pushBool(const std::string& propertyName, bool* data) { s_Instance->pushPropertyImpl(EditorSettingsPropertyType::Flag, propertyName, (void*)data); }
		static void pushString(const std::string& propertyName, std::string* data) { s_Instance->pushPropertyImpl(EditorSettingsPropertyType::String, propertyName, (void*)data); }

		static void pushProperty(EditorSettingsPropertyType type, const std::string& propertyName, void* data) { s_Instance->pushPropertyImpl(type, propertyName, data); }

	private:

		EditorSettings() = default;
		virtual ~EditorSettings() = default;

		void pushGroupImpl(const std::string& groupid) {
			EditorSettingsGroup group;
			group.groupId = groupid;

			m_Groups.push_back(group);
			isGroupPushed = true;
		}

		void popGroupImpl() {
			if (!isGroupPushed) SLD_CORE_ASSERT_FAIL("Trying to pop group but no group is pushed!");
			isGroupPushed = false;
		}

		EditorSettingsGroup& getCurrentGroup() {
			return m_Groups[m_Groups.size() - 1];
		}

		EditorSettingsPropertyList& getCurrentSubGroup() {
			EditorSettingsGroup& curGroup = getCurrentGroup();
			return curGroup.subGroups[lastSubGroupId];
		}

		void pushSubGroupImpl(const std::string& subgroupid) {
			if (!isGroupPushed) SLD_CORE_ASSERT_FAIL("Trying to push sub group but no group is pushed!");

			EditorSettingsPropertyList subgroup;
			EditorSettingsGroup& curGroup = getCurrentGroup();
			curGroup.subGroups.insert({ subgroupid, subgroup });
			isSubGroupPushed = true;
			lastSubGroupId = subgroupid;
		}

		EditorSettingsGroup* getGroupFromId(const std::string& groupid) {
			for(auto& group : m_Groups) {
				if (group.groupId == groupid) return &group;
			}
			return nullptr;
		}

		EditorSettingsPropertyList* getSubGroupFromId(EditorSettingsGroup* parent, const std::string& subgroupid) {
			for(auto it = parent->subGroups.begin(); it != parent->subGroups.end(); it++) {
				if (it->first == subgroupid) return &it->second;
			}
			return nullptr;
		}

		void popSubGroupImpl() {
			if (!isSubGroupPushed) SLD_CORE_ASSERT_FAIL("Trying to pop subgroup but no subgroup is pushed!");

			lastSubGroupId = "";
			isSubGroupPushed = false;
		}

		void pushPropertyImpl(EditorSettingsPropertyType type, const std::string& propertyName, void* data) {
			
			if (!isGroupPushed) SLD_CORE_ASSERT_FAIL("Trying to push property but no group is pushed!");

			EditorSettingsProperty prop = { type, propertyName, data };

			if(isSubGroupPushed) {
				getCurrentSubGroup().properties.push_back(prop);
			}
			else {
				getCurrentGroup().propertyList.properties.push_back(prop);
			}
		}

		// ----- Serialization/Deserialization Start ----- 

		void populateJsonWithProperties(json& j, EditorSettingsPropertyList& properties) {
			auto jsonProperties = json::array();
			for (auto& prop : properties.properties) {
				auto jsonProp = json::object();
				jsonProp["name"] = prop.name;
				switch (prop.type) {
					case EditorSettingsPropertyType::Unknown:
						jsonProp["data"] = "";
						break;

					case EditorSettingsPropertyType::Float:
						jsonProp["data"] = *((float*)prop.data);
						break;

					case EditorSettingsPropertyType::Int:
						jsonProp["data"] = *((int*)prop.data);
						break;

					case EditorSettingsPropertyType::Flag:
						jsonProp["data"] = *((bool*)prop.data);
						break;

					case EditorSettingsPropertyType::String:
						jsonProp["data"] = *((std::string*)prop.data);
						break;

				}
				jsonProperties.push_back(jsonProp);
			}

			j["properties"] = jsonProperties;
		}

		void serializeSettings() {
			if (m_Groups.size() == 0) return;
			
			auto jsonGroups = json::array();
			for(auto it = m_Groups.begin(); it != m_Groups.end(); it++) {
				auto jsonGroup = json::object();
				jsonGroup["groupid"] = it->groupId;
				populateJsonWithProperties(jsonGroup, it->propertyList);

				auto jsonSubGroups = json::array();

				if (!it->subGroups.empty()) {
					for (auto sub_it = it->subGroups.begin(); sub_it != it->subGroups.end(); sub_it++) {
						json jsonSubGroup = json::object();
						jsonSubGroup["name"] = sub_it->first;
						populateJsonWithProperties(jsonSubGroup, sub_it->second);
						jsonSubGroups.push_back(jsonSubGroup);
					}
				}

				jsonGroup["subgroups"] = jsonSubGroups;
				jsonGroups.push_back(jsonGroup);
			}

			json j;
			j["Editor_Settings"] = jsonGroups;

			std::ofstream file;
			file.open(SLD_EDITOR_SETTINGS_FILEPATH);
			file << std::setw(4) << j;
			file.close();
		}

		void loadPropertiesFromJson(json& j, EditorSettingsPropertyList& properties) {
			for(auto it = j.begin(); it != j.end(); it++) {
				auto& jsonProp = it.value();
				std::string propName = jsonProp["name"].get<std::string>();
				auto* prop = properties.getPropertyFromName(propName);
				if (prop == nullptr) continue;
				auto propType = prop->type;
				switch(propType) {
				
					case EditorSettingsPropertyType::Unknown: break;
					case EditorSettingsPropertyType::Float:
						*((float*)(prop->data)) = jsonProp["data"].get<float>();
						break;

					case EditorSettingsPropertyType::Int:
						*((int*)(prop->data)) = jsonProp["data"].get<int>();
						break;

					case EditorSettingsPropertyType::Flag:
						*((bool*)(prop->data)) = jsonProp["data"].get<bool>();
						break;

					case EditorSettingsPropertyType::String:
						*((std::string*)(prop->data)) = jsonProp["data"].get<std::string>();
						break;
				}
			}
		}

		void deserializeSettings() {
			if (!FileUtil::fileExists(SLD_EDITOR_SETTINGS_FILEPATH)) return;

			std::ifstream ifs(SLD_EDITOR_SETTINGS_FILEPATH);
			json j = json::parse(ifs);

			json groups = j["Editor_Settings"];
			for(auto it = groups.begin(); it != groups.end(); it++) {
				auto& jsonGroup = it.value();
				std::string groupid = jsonGroup["groupid"].get<std::string>();
				EditorSettingsGroup* group = getGroupFromId(groupid);
				if (group == nullptr) continue;

				loadPropertiesFromJson(jsonGroup["properties"], group->propertyList);

				auto& jsonSubGroups = jsonGroup["subgroups"];
				for (auto it = jsonSubGroups.begin(); it != jsonSubGroups.end(); it++) {
					auto& jsonSubGroup = it.value();
					std::string subGroupId = jsonSubGroup["name"].get<std::string>();
					auto* subgroup = getSubGroupFromId(group, subGroupId);
					if (subgroup == nullptr) continue;
					loadPropertiesFromJson(jsonSubGroup["properties"], *subgroup);
				}
			}
		}
		// ----- Serialization/Deserialization End ----- 

	private:
		inline static EditorSettings* s_Instance;

		friend class EditorLayer;

	private:
		std::vector<EditorSettingsGroup> m_Groups;

		bool isGroupPushed = false;
		std::string lastSubGroupId;
		bool isSubGroupPushed = false;

		friend class EditorSettingsWindow;
	};
}
