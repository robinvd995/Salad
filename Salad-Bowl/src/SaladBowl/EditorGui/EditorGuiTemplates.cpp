#include "EditorGuiTemplates.h"

#include "EditorGuiHelper.h"
#include "EditorStyle.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <string>

void Salad::EditorGui::templateContextItem(const std::string& label, const char* name, ContextItemData& data, std::function<void()> selectedFunc) {
	std::string actualLabel = std::string("##").append(label);
	ImVec2 cursorPos = ImGui::GetCursorPos();
	bool selected = false;

	float lineHeight = getLineHeight() - 4;

	if(ImGui::Selectable(actualLabel.c_str(), &selected, 0, { 280.0f, lineHeight })) {
		selectedFunc();
	}
	ImVec2 cursorPosAfter = ImGui::GetCursorPos();

	if (data.hasPrefixIcon) {
		float offy = (lineHeight - data.iconH) / 2;

		ImGui::SetCursorPos({ cursorPos.x, cursorPos.y + offy });
		uint32_t textureId = EditorGui::EditorStyle::getEditorIcons()->getRendererId();
		ImGui::Image((void*)textureId, ImVec2(data.iconW, data.iconH), ImVec2(data.iconU0, data.iconV0), ImVec2(data.iconU1, data.iconV1));
	}

	ImGui::SetCursorPos({ cursorPos.x + 30, cursorPos.y });
	ImGui::Text(name);

	if (data.hasAffix) {
		ImGui::SetCursorPos({ cursorPos.x + 200, cursorPos.y });
		ImGui::Text(data.affix);
	}

	ImGui::SetCursorPos(cursorPosAfter);
}