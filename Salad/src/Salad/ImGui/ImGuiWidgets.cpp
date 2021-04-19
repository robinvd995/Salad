#include "sldpch.h"
#include "ImGuiWidgets.h"

#include "Salad/Core/Log.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Salad {

	static char s_TextboxLabelBuffer[64];
	static char s_TextboxBuffer[256];

	float ImGuiWidgets::s_ColWidth = 100.0f;

	bool ImGuiWidgets::drawFloatSlider(const char* label, float* value, float min, float max) {
		bool changed = false;
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		float sliderWidth = ImGui::GetContentRegionAvail().x;
		ImGui::PushItemWidth(sliderWidth);
		changed = ImGui::SliderFloat("##FloatSlider", value, min, max);
		ImGui::PopItemWidth();

		ImGui::Columns(1);
		ImGui::PopID();

		return changed;
	}

	bool ImGuiWidgets::drawTextboxControl(const char* label, std::string* text, std::string* resetValue) {

		bool changed = false;
		bool hasResetButton = resetValue;

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 6.0f });

		memset(s_TextboxBuffer, 0, sizeof(s_TextboxBuffer));
		strcpy_s(s_TextboxBuffer, sizeof(s_TextboxBuffer), (*text).c_str());

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		float lineHeight = boldFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight, lineHeight };

		float filterInputWidth = ImGui::GetContentRegionAvail().x - (hasResetButton ? buttonSize.x : 0);
		ImGui::PushItemWidth(filterInputWidth);
		if (ImGui::InputText("##TextboxControl", s_TextboxBuffer, sizeof(s_TextboxBuffer))) {
			*text = std::string(s_TextboxBuffer);
			changed = true;
		}
		ImGui::PopItemWidth();

		if (hasResetButton) {
			ImGui::SameLine();
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize)) { (*text) = *resetValue; changed = true; }
			ImGui::PopFont();
		}

		ImGui::PopStyleVar();
		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool ImGuiWidgets::drawComboBox(const char* label, const char** values, int size, int& selectedId) {
		bool changed = false;

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		int contentWidth = ImGui::GetContentRegionAvailWidth();
		ImGui::PushItemWidth(contentWidth);

		const char* curString = values[selectedId];
		if (ImGui::BeginCombo("##ComboBox", curString)) {

			for (int i = 0; i < size; i++) {
				bool isSelected = selectedId == i;
				if (ImGui::Selectable(values[i], isSelected)) {
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

	bool ImGuiWidgets::drawCheckboxControl(const char* label, bool* value) {
		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		bool changed = 0 | ImGui::Checkbox("##Checkbox", value);

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

	bool ImGuiWidgets::drawFloat3Control(const char* label, float* x, float* y, float* z, float resetValue) {

		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		float lineHeight = boldFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		float buttonWidth = lineHeight + 3.0f;

		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
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

	bool ImGuiWidgets::drawFloat2Control(const char* label, float* x, float* y, bool hasReset, float resetValue) {
		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		float contentWidth = ImGui::GetContentRegionAvailWidth();
		float dragFloatWidth = (contentWidth - (hasReset ? (buttonSize.x * 2.0f) : 0.0f)) / 2.0f;

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 1.0f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });

		if (hasReset) {
			ImGui::PushFont(boldFont);
			if (ImGui::Button("X", buttonSize)) { *x = resetValue; changed = true; }
			ImGui::PopFont();
		}

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##X", x, 0.1f);
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (hasReset) {
			ImGui::PushFont(boldFont);
			if (ImGui::Button("Y", buttonSize)) { *y = resetValue; changed = true; }
			ImGui::PopFont();
		}

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

	bool ImGuiWidgets::drawFloatControl(const char* label, float* value, bool hasReset, float resetValue) {

		bool changed = false;

		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];

		ImGui::PushID(label);

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, s_ColWidth);
		ImGui::Text(label);
		ImGui::NextColumn();

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		float contentWidth = ImGui::GetContentRegionAvailWidth();
		float dragFloatWidth = contentWidth - (hasReset ? buttonSize.x : 0);

		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0.0f, 4.0f });

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.3f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.1f, 0.1f, 1.0f });

		if (hasReset) {
			ImGui::PushFont(boldFont);
			if (ImGui::Button("R", buttonSize)) { *value = resetValue; changed = true; }
			ImGui::PopFont();
		}

		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::PushItemWidth(dragFloatWidth);
		changed |= ImGui::DragFloat("##FLOAT", value, 0.1f);
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return changed;
	}

}