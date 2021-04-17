#pragma once

#include "Salad/Scene/Entity.h"

namespace Salad {

	class ScenePropertiesPanel {

	public:
		ScenePropertiesPanel() = default;

		void onImGuiRender();

	private:
		template<typename T>
		void drawComponent(const char* label, Entity entity, std::function<void(Entity)> lambda);
	};
}
