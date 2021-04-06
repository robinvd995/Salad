#pragma once

#include "Salad/Scene/Entity.h"

namespace Salad {

	class ScenePropertiesPanel {

	public:
		ScenePropertiesPanel() = default;

		void onImGuiRender(Entity entity);

	private:
		template<typename T>
		void drawComponent(const char* label, Entity entity, std::function<void(Entity)> lambda);
		/*static bool drawCheckboxControl(const char* label, bool* value);

		static bool drawComboBox(const char* label, const char** values, int size, int& selectedId);

		static bool drawFloat3Control(const char* label, float* x, float* y, float* z, float resetValue = 0.0f);
		static bool drawFloat2Control(const char* label, float* x, float* y, float resetValue = 0.0f);
		static bool drawFloatControl(const char* label, float* value, float resetValue = 0.0f);*/
	};
}
