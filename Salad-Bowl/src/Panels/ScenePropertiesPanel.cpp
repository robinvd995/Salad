#include "ScenePropertiesPanel.h"

#include "Salad/Scene/Components.h"

#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui_internal.h>

#include "Salad/ImGui/ImGuiWidgets.h"

#include "EditorSelectionContext.h"
#include "Assets/AssetLoader.h"

namespace Salad {

	static char s_TagBuffer[256];
	static const char* s_CameraProjectionStrings[2] = { "Perspective", "Orthographic" };
	static const char* s_TextureMinFilterStyleStrings[6] = { "Linear", "Nearest", "Nearest Mipmap Nearest", 
		"Linear Mipmap Nearest", "Nearest Mipmap Linear", "Linear Mipmap Linear" };

	static const char* s_TextureMagFilterStyleStrings[2] = { "Linear", "Nearest" };
	static const char* s_TextureWrapStyleStrings[5] = { "Repeat", "Clamp Edge", "Clamp Border", "Mirrored Repeat", "Mirrored Clamp Edge" };

	namespace Util {

		void drawTreeNode(const char* label, void* id, std::function<void()> lambda) {
			ImGui::PushStyleColor(ImGuiCol_Header, { 0.15f, 0.2f, 0.25f, 1.0f });
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });

			auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
			bool open = ImGui::TreeNodeEx(id, flags, label);
			/*ImGui::SameLine(ImGui::GetContentRegionAvail().x + 8.0f);

			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[1];

			ImGui::PushFont(boldFont);
			ImGui::Button("+", ImVec2{ 25.0f, 25.0f });
			ImGui::PopFont();*/

			if (open) {
				ImGui::Dummy({ 0.0f, 1.0f });
				lambda();
				ImGui::Dummy({ 0.0f, 4.0f });
				ImGui::TreePop();
			}

			ImGui::PopStyleVar();
			ImGui::PopStyleColor(1);
		}

		template<typename T>
		void drawEntityComponent(const char* label, Entity entity, std::function<void()> lambda) {
			if (entity.hasComponent<T>()) {
				//drawTreeNode(label, (void*)typeid(T).hash_code(), lambda);
				ImGui::PushStyleColor(ImGuiCol_Header, { 0.15f, 0.2f, 0.25f, 1.0f });
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4.0f, 4.0f });

				auto flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_AllowItemOverlap;
				bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), flags, label);
				ImGui::SameLine(ImGui::GetContentRegionAvail().x + 8.0f);

				ImGuiIO& io = ImGui::GetIO();
				auto& boldFont = io.Fonts->Fonts[1];

				ImGui::PushFont(boldFont);
				ImGui::Button("+", ImVec2{ 25.0f, 25.0f });
				ImGui::PopFont();

				if (open) {
					ImGui::Dummy({ 0.0f, 1.0f });
					lambda();
					ImGui::Dummy({ 0.0f, 4.0f });
					ImGui::TreePop();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleColor(1);
			}
		}

		const char* shaderTypeToString(EditorShaderDataType datatype) {
			switch (datatype) {
				case EditorShaderDataType::Float: return "Float";
				case EditorShaderDataType::Int: return "Int";
				case EditorShaderDataType::Mat2: return "Mat2";
				case EditorShaderDataType::Mat3: return "Mat3";
				case EditorShaderDataType::Mat4: return "Mat4";
				case EditorShaderDataType::Vec2: return "Vec2";
				case EditorShaderDataType::Vec3: return "Vec3";
				case EditorShaderDataType::Vec4: return "Vec4";
				case EditorShaderDataType::Sampler2D: return "Texture2D";

				case EditorShaderDataType::None: return "UNKNOWN!";
				default: return "UNKNOWN!";
			}
		}

		void drawShaderVariables(const char* label, std::vector<ShaderVariable>& variables) {
			ImGuiIO& io = ImGui::GetIO();
			auto& boldFont = io.Fonts->Fonts[1];

			ImGui::PushFont(boldFont);
			ImGui::Text(label);
			ImGui::PopFont();

			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 200.0f);
			for (ShaderVariable inputVar : variables) {
				ImGui::Text(inputVar.identifier.c_str());
				ImGui::NextColumn();
				ImGui::Text(shaderTypeToString(inputVar.type));
				ImGui::NextColumn();
			}
			ImGui::Columns(1);
		}

		void drawShaderStage(const char* label, ShaderStage& stage) {
			drawTreeNode(label, (void*)(static_cast<int>(stage.shaderType)), [&stage]() {

				drawShaderVariables("Input", stage.inputs);
				ImGui::Dummy({ 0.0f, 4.0f });
				drawShaderVariables("Output", stage.outputs);
				ImGui::Dummy({ 0.0f, 4.0f });
				drawShaderVariables("Uniforms", stage.uniforms);
			});
		}
	}

	

	void ScenePropertiesPanel::renderEntityProperties() {
		EntitySelectionContext* context = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
		Entity entity = context->getSelectedEntity();

		if (entity.isValid()) {
			// Render Tag Component
			std::string tagResetValue = "";
			auto& tag = entity.getComponent<TagComponent>().Tag;
			ImGuiWidgets::drawTextboxControl("Tag", &tag);

			ImGui::Separator();
			ImGui::Dummy({ 0.0f, 4.0f });

			// Render Transform Component
			Util::drawEntityComponent<TransformComponent>("Transform", entity, [&entity]() {
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
			Util::drawEntityComponent<CameraComponent>("Camera", entity, [&entity]() {

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

			Util::drawEntityComponent<MeshComponent>("Mesh Renderer", entity, [&entity]() {
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
			ImGui::Text("INVALID ENTITY");
		}
	}

	void ScenePropertiesPanel::renderEditorShaderProperties() {
		ShaderSelectionContext* context = EditorSelectionContext::getSelectionContext<ShaderSelectionContext>();
		EditorShader& shader = context->getEditorShader();
		ImGui::Text(shader.getFilePath().c_str());

		if(shader.hasStage(ShaderStageType::Vertex)) {
			ShaderStage& vertexStage = shader.getStage(ShaderStageType::Vertex);
			Util::drawShaderStage("Vertex Shader", vertexStage);
		}

		if (shader.hasStage(ShaderStageType::Fragment)) {
			ShaderStage& fragmentStage = shader.getStage(ShaderStageType::Fragment);
			Util::drawShaderStage("Fragment Shader", fragmentStage);
		}
	}

	void ScenePropertiesPanel::renderEditorTextureProperties() {
		TextureSelectionContext* context = EditorSelectionContext::getSelectionContext<TextureSelectionContext>();
		EditorTexture& texture = context->getTexture();

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, 100.0f);
		ImGui::Text("Texture");
		ImGui::NextColumn();
		ImGui::Text(texture.getFileName().c_str());
		ImGui::Columns(1);

		ImGui::Separator();

		Util::drawTreeNode("Texture Preview", (void*)0, [&texture]() {
			float texSize = 256;
			ImGui::Image((void*)texture.getTextureRenderId(), { texSize, texSize }, { 0, 0 }, { 1, 1 }, { 1,1,1,1 }, { 0.75f,0.75f,0.75f,1 });
		});

		Util::drawTreeNode("Texture Properties", (void*)1, [&texture]() {
			ImGui::Columns(2);
			ImGui::SetColumnWidth(0, 100.0f);
			ImGui::Text("Dimensions");
			ImGui::NextColumn();
			std::string dimString = std::to_string(texture.getTextureWidth()).append(" x ").append(std::to_string(texture.getTextureHeight()));
			ImGui::Text(dimString.c_str());
			ImGui::Columns(1);

			ImGui::Dummy({ 0.0f, 4.0f });

			int minFilter = static_cast<int>(texture.getTextureMinFilter());
			int magFilter = static_cast<int>(texture.getTextureMagFilter());
			int wrapS = static_cast<int>(texture.getTextureWrapS());
			int wrapT = static_cast<int>(texture.getTextureWrapT());
			int wrapR = static_cast<int>(texture.getTextureWrapR());

			bool textureChanged = false;
			textureChanged |= ImGuiWidgets::drawComboBox("Min Filter", s_TextureMinFilterStyleStrings, 6, minFilter);
			textureChanged |= ImGuiWidgets::drawComboBox("Mag Filter", s_TextureMagFilterStyleStrings, 2, magFilter);
			textureChanged |= ImGuiWidgets::drawComboBox("Wrap S", s_TextureWrapStyleStrings, 5, wrapS);
			textureChanged |= ImGuiWidgets::drawComboBox("Wrap T", s_TextureWrapStyleStrings, 5, wrapT);
			textureChanged |= ImGuiWidgets::drawComboBox("Wrap R", s_TextureWrapStyleStrings, 5, wrapR);

			if (textureChanged) {
				texture.setTextureMinFilter(static_cast<TextureMinFilterSpecification>(minFilter));
				texture.setTextureMagFilter(static_cast<TextureMagFilterSpecification>(magFilter));
				texture.setTextureWrapS(static_cast<TextureWrapSpecification>(wrapS));
				texture.setTextureWrapT(static_cast<TextureWrapSpecification>(wrapT));
				texture.setTextureWrapR(static_cast<TextureWrapSpecification>(wrapR));
				Asset::saveEditorTexture(texture);
			}
		});
		
	}

	void ScenePropertiesPanel::onImGuiRender() {

		ImGui::Begin("Properties");
		EditorSelectionContextType selectionType = EditorSelectionContext::getSelectionContextType();

		switch(selectionType) {
			case EditorSelectionContextType::Entity: renderEntityProperties(); break;
			case EditorSelectionContextType::Shader: renderEditorShaderProperties(); break;
			case EditorSelectionContextType::Texture: renderEditorTextureProperties(); break;

			default: ImGui::Text("Nothing to display"); break;
		}
		
		ImGui::End();
	}

}
