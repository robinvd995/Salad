#pragma once

#include "Salad/Scene/Entity.h"

namespace Salad::EditorGui {

	class ScenePropertiesPanel {

	public:
		ScenePropertiesPanel() = default;

		void init();
		void onImGuiRender();

	private:
		void renderEntityProperties();
		void renderEditorShaderProperties();
		void renderEditorTextureProperties();

		/*template<typename T>
		void drawComponent(const char* label, Entity entity, std::function<void(Entity)> lambda);*/
	};
}
