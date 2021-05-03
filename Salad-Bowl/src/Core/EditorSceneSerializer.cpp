#include "EditorSceneSerializer.h"

#include "Util/Xml.h"
#include "Util/FileUtils.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <sstream>

namespace Salad {

	using namespace Xml;

	namespace Internal {

		constexpr char transformComponentId[] = "component_transform";
		constexpr char relationComponentId[] = "component_relation";
		constexpr char cameraComponentId[] = "component_camera";
		constexpr char meshComponentId[] = "component_mesh";
		constexpr char nativeScriptComponentId[] = "component_native_script";

		template<typename T>
		inline void serializeComponent(const char* componentName, Entity& entity, XmlDocument& doc, XmlNode& entityNode, std::function<void(XmlNode&, T&)> func) {
			if (entity.hasComponent<T>()) {
				T& component = entity.getComponent<T>();
				XmlNode componentNode = doc.allocateNode(componentName);
				entityNode.appendNode(componentNode);

				func(componentNode, component);
			}
		}

		inline std::string serializeVec3(glm::vec3& value) {
			std::string s = std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z);
			return s;
		}

		inline std::string serializeQuat(glm::quat& value) {
			std::string s = std::to_string(value.x) + " " + std::to_string(value.y) + " " + std::to_string(value.z) + " " + std::to_string(value.w);
			return s;
		}
	}

	using namespace Internal;

	void EditorSceneSerializer::serializeScene(const std::string& filepath) {
		XmlDocument doc = createDocument("scene");

		XmlNode entitiesNode = doc.allocateNode("entities");
		doc.appendNodeToRoot(entitiesNode);

		getSceneEntityRegistry().each([this, &doc, &entitiesNode](auto p_entity) {
			Entity entity{ p_entity, m_Scene };

			XmlNode entityNode = doc.allocateNode("entity");
			entityNode.appendAttribute("id", 64);

			TagComponent& tagComponent = entity.getComponent<TagComponent>();
			entityNode.appendAttribute("tag", tagComponent.Tag.c_str());

			entitiesNode.appendNode(entityNode);

			serializeComponent<TransformComponent>(transformComponentId, entity, doc, entityNode, [&doc](XmlNode& node, TransformComponent& component) {
				node.appendNode(doc.allocateNode("position").setBufferedValue(serializeVec3(component.Transform.getPosition())));
				node.appendNode(doc.allocateNode("rotation").setBufferedValue(serializeQuat(component.Transform.getOrientation())));
				node.appendNode(doc.allocateNode("scale").setBufferedValue(serializeVec3(component.Transform.getScale())));
			});

			serializeComponent<RelationshipComponent>(relationComponentId, entity, doc, entityNode, [&doc](XmlNode& node, RelationshipComponent& component) {
				node.appendAttribute("parent", 128); // TODO
				node.appendAttribute("depth", (int)component.hierarchyDepth);
				for (auto child : component.children) {
					node.appendNode(doc.allocateNode("child").setBufferedValue("child_id")); // TODO
				}
			});

			serializeComponent<CameraComponent>(cameraComponentId, entity, doc, entityNode, [&doc](XmlNode& node, CameraComponent& component) {
				node.appendAttribute("projection_type", static_cast<int>(component.Camera.getCameraProjectionType()));
				node.appendAttribute("primary", component.Camera.isPrimaryCamera());

				PerspectiveCameraProperties& pProps = component.Camera.getPerspectiveProperties();
				XmlNode& perspectiveNode = doc.allocateNode("perspective")
					.appendAttribute("fov", pProps.Fov)
					.appendAttribute("aspect_ratio", pProps.AspectRatio)
					.appendAttribute("near_clip", pProps.NearClip)
					.appendAttribute("far_clip", pProps.FarClip);
				node.appendNode(perspectiveNode);

				OrthographicCameraProperties& oProps = component.Camera.getOrthographicProperties();
				XmlNode& orthographicNode = doc.allocateNode("orthographic")
					.appendAttribute("left", oProps.Left).appendAttribute("right", oProps.Right)
					.appendAttribute("top", oProps.Top).appendAttribute("bottom", oProps.Bottom)
					.appendAttribute("z_near", oProps.ZNear).appendAttribute("z_far", oProps.ZFar);
				node.appendNode(orthographicNode);
			});

			serializeComponent<MeshComponent>(meshComponentId, entity, doc, entityNode, [&doc](XmlNode& node, MeshComponent component) {
				node.appendBufferedAttribute("mesh_resource", component.meshResourceId) // TODO
					.appendBufferedAttribute("shader_resource", component.shaderResourceId) // TODO
					.appendBufferedAttribute("texture_resource", component.textureResourceId); // TODO
			});

			serializeComponent<NativeScriptComponent>(nativeScriptComponentId, entity, doc, entityNode, [&doc](XmlNode& node, NativeScriptComponent component) {
				node.appendAttribute("script", "somescript"); // TODO
			});
		});

		FileUtil::createDirectories(FileUtil::popFileName(filepath));

		std::ofstream file(filepath);
		file << doc;
		file.close();
		doc.close();
	}

	void EditorSceneSerializer::deserializeScene(const std::string& filepath) {
		// TODO
	}

}
