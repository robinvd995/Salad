#include "sldpch.h"
#include "SceneSerializer.h"

#include "json.hpp"

#include "Entity.h"
#include "Components.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include <iomanip>
#include <fstream>

using json = nlohmann::json;

// custom type serialization/deserialization
namespace glm {

	// Vec3 Serialization
	void to_json(json& j, const glm::vec3& v) {
		j = { { "x", v.x }, { "y", v.y }, { "z", v.z } };
	}

	void from_json(const json& j, glm::vec3& p) {
		p.x = j.at("x").get<float>();
		p.y = j.at("y").get<float>();
		p.z = j.at("z").get<float>();
	}

	//Quaternion serialization
	void to_json(json& j, const glm::quat& q) {
		j = { { "x", q.x }, { "y", q.y }, { "z", q.z }, {"w", q.w} };
	}

	void from_json(const json& j, glm::quat& q) {
		q.x = j.at("x").get<float>();
		q.y = j.at("y").get<float>();
		q.z = j.at("z").get<float>();
		q.w = j.at("w").get<float>();
	}
}

namespace Salad {

	template<typename T>
	static void serializeComponent(const std::string& componentName, Entity& entity, json& jEntity, std::function<void(json&, T&)> func) {
		if (entity.hasComponent<T>()) {
			T& component = entity.getComponent<T>();
			auto jComponent = json::object();

			func(jComponent, component);

			jEntity[componentName] = jComponent;
		}
	}

	void SceneSerializer::serializeScene(const std::string& filepath) {
		json j;

		// Serialize entities
		auto jsonEntities = json::array();
		
		m_Scene->m_Registry.each([this, &jsonEntities](auto p_entity) {
			auto jEntity = json::object();

			Entity entity{ p_entity, m_Scene };
			jEntity["id"] = 215487643135;

			serializeComponent<TagComponent>("TagComponent", entity, jEntity, [](json& jComponent, TagComponent& component) {
				jComponent["Tag"] = component.Tag;
			});

			serializeComponent<TransformComponent>("TransformComponent", entity, jEntity, [](json& jComponent, TransformComponent& component) {
				jComponent["position"] = component.Transform.getPosition();
				jComponent["orientation"] = component.Transform.getOrientation();
				jComponent["scale"] = component.Transform.getScale();
			});

			serializeComponent<RelationshipComponent>("RelationshipComponent", entity, jEntity, [](json& jComponent, RelationshipComponent& component) {
				jComponent["parentid"] = 65871244587; // component.parentid
				jComponent["hierarchyDepth"] = component.hierarchyDepth;
				auto childrenArray = json::array();

				for (auto child : component.children) {
					childrenArray.push_back(87458455); // child;
				}

				jComponent["children"] = childrenArray;
			});

			serializeComponent<CameraComponent>("CameraComponent", entity, jEntity, [](json& jComponent, CameraComponent& component) {
				jComponent["ProjectionType"] = component.Camera.getCameraProjectionType();
				jComponent["PrimaryCamera"] = component.Camera.isPrimaryCamera();

				auto jPerspective = json::object();
				PerspectiveCameraProperties& pProps = component.Camera.getPerspectiveProperties();

				jPerspective["Fov"] = pProps.Fov;
				jPerspective["AspectRatio"] = pProps.AspectRatio;
				jPerspective["NearClip"] = pProps.NearClip;
				jPerspective["FarClip"] = pProps.FarClip;

				jComponent["Perspective"] = jPerspective;

				auto jOrtho = json::object();
				OrthographicCameraProperties& oProps = component.Camera.getOrthographicProperties();

				jOrtho["Left"] = oProps.Left;
				jOrtho["Right"] = oProps.Right;
				jOrtho["Top"] = oProps.Top;
				jOrtho["Bottom"] = oProps.Bottom;
				jOrtho["ZFar"] = oProps.ZFar;
				jOrtho["ZNear"] = oProps.ZNear;

				jComponent["Ortho"] = jOrtho;
			});

			serializeComponent<MeshComponent>("MeshComponent", entity, jEntity, [](json& jComponent, MeshComponent& component) {
				jComponent["Model"] = "Not yet implemented";
				jComponent["Shader"] = "Not yet implemented";
				jComponent["Texture"] = "Not yet implemented";
			});

			serializeComponent<NativeScriptComponent>("NativeScriptComponent", entity, jEntity, [](json& jComponent, NativeScriptComponent& component) {
				jComponent["Script"] = "Not yet implemented";
			});

			/*
			template: serializeComponent<INSERT_COMPONENT>("INSERT_COMPONENT", entity, jEntity, [](json& jComponent, INSERT_COMPONENT& component) {});
			*/

			jsonEntities.push_back(jEntity);
		});
		j["Entities"] = jsonEntities;

		std::ofstream file;
		file.open(filepath);
		file << std::setw(4) << j;
		file.close();
	}

	void SceneSerializer::deserializeScene(const std::string& filepath) {

	}

}
