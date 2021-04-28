#include "EditorStyle.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <cstdint>

namespace Salad::EditorGui {

	//EditorStyle* EditorStyle::s_Instance = new EditorStyle();

	void EditorStyle::setEditorStyleImpl(EditorUIStyle styletype) {

		auto& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		switch(styletype) {
			case EditorUIStyle::Default: break;

			case EditorUIStyle::DarkCharcoal: {
				// Source=https://github.com/ocornut/imgui/issues/707#issuecomment-463758243
				colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
				colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
				colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
				colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
				colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
				colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
				colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
				colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
				colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
				colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
				colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
				colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
				colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
				colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
				colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
				colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
				colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
				colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
				colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
				colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
				colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
				colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
				colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
				colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
				colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
				colors[ImGuiCol_Separator] = colors[ImGuiCol_Border];
				colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
				colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
				colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
				colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
				colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
				colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
				colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
				colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
				colors[ImGuiCol_DockingPreview] = ImVec4(1.000f, 0.391f, 0.000f, 0.781f);
				colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
				colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
				colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
				colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
				colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
				colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
				colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

				style.ChildRounding = 4.0f;
				style.FrameBorderSize = 1.0f;
				style.FrameRounding = 2.0f;
				style.GrabMinSize = 7.0f;
				style.PopupRounding = 2.0f;
				style.ScrollbarRounding = 12.0f;
				style.ScrollbarSize = 13.0f;
				style.TabBorderSize = 1.0f;
				style.TabRounding = 0.0f;
				style.WindowRounding = 4.0f;

			} break;

			case EditorUIStyle::VisualStudio: {
				// Source=https://github.com/ocornut/imgui/issues/707#issuecomment-670976818
				constexpr auto ColorFromBytes = [](uint8_t r, uint8_t g, uint8_t b) {
					return ImVec4((float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f);
				};

				const ImVec4 bgColor = ColorFromBytes(37, 37, 38);
				const ImVec4 lightBgColor = ColorFromBytes(82, 82, 85);
				const ImVec4 veryLightBgColor = ColorFromBytes(90, 90, 95);

				const ImVec4 panelColor = ColorFromBytes(51, 51, 55);
				const ImVec4 panelHoverColor = ColorFromBytes(29, 151, 236);
				const ImVec4 panelActiveColor = ColorFromBytes(0, 119, 200);

				const ImVec4 textColor = ColorFromBytes(255, 255, 255);
				const ImVec4 textDisabledColor = ColorFromBytes(151, 151, 151);
				const ImVec4 borderColor = ColorFromBytes(78, 78, 78);

				colors[ImGuiCol_Text] = textColor;
				colors[ImGuiCol_TextDisabled] = textDisabledColor;
				colors[ImGuiCol_TextSelectedBg] = panelActiveColor;
				colors[ImGuiCol_WindowBg] = bgColor;
				colors[ImGuiCol_ChildBg] = bgColor;
				colors[ImGuiCol_PopupBg] = bgColor;
				colors[ImGuiCol_Border] = borderColor;
				colors[ImGuiCol_BorderShadow] = borderColor;
				colors[ImGuiCol_FrameBg] = panelColor;
				colors[ImGuiCol_FrameBgHovered] = panelHoverColor;
				colors[ImGuiCol_FrameBgActive] = panelActiveColor;
				colors[ImGuiCol_TitleBg] = bgColor;
				colors[ImGuiCol_TitleBgActive] = bgColor;
				colors[ImGuiCol_TitleBgCollapsed] = bgColor;
				colors[ImGuiCol_MenuBarBg] = panelColor;
				colors[ImGuiCol_ScrollbarBg] = panelColor;
				colors[ImGuiCol_ScrollbarGrab] = lightBgColor;
				colors[ImGuiCol_ScrollbarGrabHovered] = veryLightBgColor;
				colors[ImGuiCol_ScrollbarGrabActive] = veryLightBgColor;
				colors[ImGuiCol_CheckMark] = panelActiveColor;
				colors[ImGuiCol_SliderGrab] = panelHoverColor;
				colors[ImGuiCol_SliderGrabActive] = panelActiveColor;
				colors[ImGuiCol_Button] = panelColor;
				colors[ImGuiCol_ButtonHovered] = panelHoverColor;
				colors[ImGuiCol_ButtonActive] = panelHoverColor;
				colors[ImGuiCol_Header] = panelColor;
				colors[ImGuiCol_HeaderHovered] = panelHoverColor;
				colors[ImGuiCol_HeaderActive] = panelActiveColor;
				colors[ImGuiCol_Separator] = borderColor;
				colors[ImGuiCol_SeparatorHovered] = borderColor;
				colors[ImGuiCol_SeparatorActive] = borderColor;
				colors[ImGuiCol_ResizeGrip] = bgColor;
				colors[ImGuiCol_ResizeGripHovered] = panelColor;
				colors[ImGuiCol_ResizeGripActive] = lightBgColor;
				colors[ImGuiCol_PlotLines] = panelActiveColor;
				colors[ImGuiCol_PlotLinesHovered] = panelHoverColor;
				colors[ImGuiCol_PlotHistogram] = panelActiveColor;
				colors[ImGuiCol_PlotHistogramHovered] = panelHoverColor;
				colors[ImGuiCol_ModalWindowDarkening] = bgColor;
				colors[ImGuiCol_DragDropTarget] = bgColor;
				colors[ImGuiCol_NavHighlight] = bgColor;
				colors[ImGuiCol_DockingPreview] = panelActiveColor;
				colors[ImGuiCol_Tab] = bgColor;
				colors[ImGuiCol_TabActive] = panelActiveColor;
				colors[ImGuiCol_TabUnfocused] = bgColor;
				colors[ImGuiCol_TabUnfocusedActive] = lightBgColor;
				colors[ImGuiCol_TabHovered] = panelHoverColor;

				style.WindowRounding = 0.0f;
				style.ChildRounding = 0.0f;
				style.FrameRounding = 0.0f;
				style.GrabRounding = 0.0f;
				style.PopupRounding = 0.0f;
				style.ScrollbarRounding = 0.0f;
				style.TabRounding = 0.0f;

			} break;
		}
	}

	void EditorStyle::loadEditorIconsImpl(const std::string& filepath) {
		m_EditorIcons = Texture2D::create(filepath, TextureFilterWrapSpecification());
	}

	void EditorStyle::loadFileIconsImpl(const std::string& filepath) {
		m_FileIcons = Texture2D::create(filepath, TextureFilterWrapSpecification());
	}
}