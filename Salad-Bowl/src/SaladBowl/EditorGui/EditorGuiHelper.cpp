#include "EditorGuiHelper.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Salad::EditorGui {

	float getLineHeight() {
		ImGuiIO& io = ImGui::GetIO();
		auto& boldFont = io.Fonts->Fonts[1];
		float lineHeight = boldFont->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		return lineHeight;
	}
}
