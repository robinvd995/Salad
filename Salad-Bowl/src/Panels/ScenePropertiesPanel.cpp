#include "ScenePropertiesPanel.h"

#include "Salad/Scene/Components.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

#include "Salad/ImGui/ImGuiWidgets.h"
#include "EditorSelectionContext.h"

namespace Salad {

	static char s_TagBuffer[256];
	static const char* s_CameraProjectionStrings[2] = { "Perspective", "Orthographic" };

	template<typename T>
	void ScenePropertiesPanel::drawComponent(const char* label, Entity entity, std::function<void(Entity)> lambda) {

		if (entity.hasComponent<T>()) {

			ImGui::PushStyleColor(ImGuiCol_Header, { 0.15f, 0.2f, 0.25f, 1.0f });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });

			auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
			bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label);
			ImGui::SameLine(ImGui::GetContentRegionAvail().x + 8.0f);

			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[1];

			ImGui::PushFont(boldFont);
			ImGui::Button("+", ImVec2{25.0f, 25.0f});
			ImGui::PopFont();

			if (open) {
				ImGui::Dummy({ 0.0f, 1.0f });
				lambda(entity);
				ImGui::Dummy({ 0.0f, 4.0f });
				ImGui::TreePop();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(1);
		}
	}

	void ScenePropertiesPanel::onImGuiRender() {

		ImGui::Begin("Properties");

		if(EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
			EntitySelectionContext* context = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
			Entity entity = context->getSelectedEntity();
			
			if(entity.isValid()) {
				// Render Tag Component
				std::string tagResetValue = "";
				auto& tag = entity.getComponent<TagComponent>().Tag;
				ImGuiWidgets::drawTextboxControl("Tag", &tag);

				ImGui::Separator();
				ImGui::Dummy({ 0.0f, 4.0f });

				// Render Transform Component
				drawComponent<TransformComponent>("Transform", entity, [](Entity entity) {
					auto& transform = entity.getComponent<TransformComponent>().Transform;

					glm::vec3 position = transform.getPosition();
					glm::vec3 rotation = glm::degrees(glm::eulerAngles(transform.getOrientation()));
					glm::vec3 scale = transform.getScale();

					bool changed = false;
					changed |= ImGuiWidgets::drawFloat3Control("Position", &position.x, &position.y, &position.z);
					changed |= ImGuiWidgets::drawFloat3Control("Rotation", &rotation.x, &rotation.y, &rotation.z);
					changed |= ImGuiWidgets::drawFloat3Control("Scale", &scale.x, &scale.y, &scale.z, 1.0f);
					if (changed) transform.set(position, glm::quat(glm::radians(rotation)), scale);
				});

				// Render Camera Component
				drawComponent<CameraComponent>("Camera", entity, [](Entity entity) {

					auto& cameraComponent = entity.getComponent<CameraComponent>();
					Camera& cam = cameraComponent.Camera;
					int curProjectionTypeId = (int)cam.getCameraProjectionType();

					bool primaryCamera = cam.isPrimaryCamera();
					if (ImGuiWidgets::drawCheckboxControl("Primary", &primaryCamera)) {
						cam.setPrimaryCamera(primaryCamera);
					}

					bool cameraChangedFlag = false;

					cameraChangedFlag = ImGuiWidgets::drawComboBox("Projection", s_CameraProjectionStrings, 2, curProjectionTypeId);
					if (cameraChangedFlag) cam.setProjectionType((Camera::CameraType)curProjectionTypeId);

					// PERSPECTIVE PROPERTIES
					if (curProjectionTypeId == 0) {
						auto& props = cam.getPerspectiveProperties();
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Fov", &props.Fov, 65.0f); //ImGui::DragFloat("Fov",        &props.Fov);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Near Clip", &props.NearClip, 0.1f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Far Clip", &props.FarClip, 1000.0f);
					}

					// ORTHOGRAPHIC PROPERTIES
					if (curProjectionTypeId == 1) {
						auto& props = cam.getOrthographicProperties();
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Left", &props.Left, -16.0f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Right", &props.Right, 16.0f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Top", &props.Top, 9.0f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Bottom", &props.Bottom, -9.0f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("ZNear", &props.ZNear, -1.0f);
						cameraChangedFlag |= ImGuiWidgets::drawFloatControl("ZFar", &props.ZFar, 1.0f);
					}

					if (cameraChangedFlag) cam.recalculateViewMatrix();
				});

				drawComponent<MeshComponent>("Mesh Renderer", entity, [](Entity entity) {
					auto& component = entity.getComponent<MeshComponent>();
					ImGuiWidgets::drawTextboxControl("Mesh", &component.meshIdentifier);
					ImGuiWidgets::drawTextboxControl("Material", &component.materialIdentifier);
				});

				if (ImGui::Button("Add Component")) {
					ImGui::OpenPopup("AddComponentPopup");
				}

				if (ImGui::BeginPopup("AddComponentPopup")) {

					if (ImGui::MenuItem("Camera")) { entity.addComponent<CameraComponent>(); }

					ImGui::EndPopup();
				}
			}
			else {
				ImGui::Text("Nothing to display!");
			}
		}

			
		ImGui::End();
	}

}
