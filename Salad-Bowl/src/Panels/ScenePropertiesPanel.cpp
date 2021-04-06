#include "ScenePropertiesPanel.h"

#include "Salad/Scene/Components.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

#include "Util/ImGuiWidgets.h"

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

	void ScenePropertiesPanel::onImGuiRender(Entity entity) {

		ImGui::Begin("Properties");
		if(entity.isValid()) {			

			// Render Tag Component
			std::string tagResetValue = "";
			auto& tag = entity.getComponent<TagComponent>().Tag;
			ImGuiWidgets::drawTextboxControl("Tag", &tag);

			ImGui::Separator();
			ImGui::Dummy({ 0.0f, 4.0f });

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
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Fov",        &props.Fov,       65.0f); //ImGui::DragFloat("Fov",        &props.Fov);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Near Clip",  &props.NearClip,  0.1f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Far Clip",   &props.FarClip,   1000.0f);
				}

					// ORTHOGRAPHIC PROPERTIES
				if (curProjectionTypeId == 1) {
					auto& props = cam.getOrthographicProperties();
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Left",       &props.Left,   -16.0f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Right",      &props.Right,   16.0f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Top",        &props.Top,     9.0f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("Bottom",     &props.Bottom, -9.0f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("ZNear",      &props.ZNear,  -1.0f);
					cameraChangedFlag |= ImGuiWidgets::drawFloatControl("ZFar",       &props.ZFar,    1.0f);
				}

				if (cameraChangedFlag) cam.recalculateViewMatrix();
			});

			if(ImGui::Button("Add Component")) {
				ImGui::OpenPopup("AddComponentPopup");
			}

			if(ImGui::BeginPopup("AddComponentPopup")) {
				
				if (ImGui::MenuItem("Camera")) { entity.addComponent<CameraComponent>(); }

				ImGui::EndPopup();
			}
		}
		else {
			ImGui::Text("Nothing to display!");
		}
		ImGui::End();
	}

	/*bool ScenePropertiesPanel::drawComboBox(const char* label, const char** values, int size, int& selectedId) {
		bool changed = false;

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();

		int contentWidth = ImGui::GetContentRegionAvailWidth();
		ImGui::PushItemWidth(contentWidth);

		const char* curString = values[selectedId];
		if (ImGui::BeginCombo("##ComboBox", curString)) {

			for (int i = 0; i < size; i++) {
				bool isSelected = selectedId == i;
				if (ImGui::Selectable(values[i], isSelected)) {
					SLD_CORE_INFO(i);
					selectedId = i;
					changed = true;
				}

				if (isSelected) ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	bool ScenePropertiesPanel::drawCheckboxControl(const char* label, bool* value) {
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();

		bool changed = 0 | ImGui::Checkbox("##Checkbox", value);

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool ScenePropertiesPanel::drawFloat3Control(const char* label, float* x, float* y, float* z, float resetValue) {

		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		float lineHeight = boldFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		float buttonWidth = lineHeight + 3.0f;

		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushID(label);
		
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();
		
		float contentWidth = ImGui::GetContentRegionAvailWidth();
		float dragFloatWidth = (contentWidth - (buttonWidth * 3.0f)) / 3.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 1.0f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.1f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) { *x = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##X", x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.7f, 0.1f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) { *y = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##Y", y, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.8f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize)) { *z = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##Z", z, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool ScenePropertiesPanel::drawFloat2Control(const char* label, float* x, float* y, float resetValue) {
		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		float contentWidth = ImGui::GetContentRegionAvailWidth();
		float dragFloatWidth = (contentWidth - (buttonSize.x * 2.0f)) / 2.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 1.0f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize)) { *x = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##X", x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize)) { *y = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##Y", y, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleColor(3);

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool ScenePropertiesPanel::drawFloatControl(const char* label, float* value, float resetValue) {

		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text(label);
		ImGui::NextColumn();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		float contentWidth = ImGui::GetContentRegionAvailWidth();
		float dragFloatWidth = contentWidth - buttonSize.x;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 1.0f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });

		ImGui::PushFont(boldFont);
		if (ImGui::Button("R", buttonSize)) { *value = resetValue; changed = true; }
		ImGui::PopFont();

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##FLOAT", value, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}*/

}
