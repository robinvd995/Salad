#pragma once

#include <string>

namespace Salad {

	class ImGuiWidgets {

	public:
		static bool drawFloatSlider(const char* label, float* value, float min, float max);
		static bool drawTextboxControl(const char* label, std::string* text, std::string* resetValue = nullptr);
		static bool drawCheckboxControl(const char* label, bool* value);
		static bool drawComboBox(const char* label, const char** values, int size, int& selectedId);
		static bool drawFloat3Control(const char* label, float* x, float* y, float* z, float resetValue = 0.0f);
		static bool drawFloat2Control(const char* label, float* x, float* y, bool hasReset = false, float resetValue = 0.0f);
		static bool drawFloatControl(const char* label, float* value, bool hasReset = false, float resetValue = 0.0f);

	public:
		static float getColWidth() { return s_ColWidth; }
		static void setColWidth(float width) { s_ColWidth = width; }

	private:
		static float s_ColWidth;
	};
}
