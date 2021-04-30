#pragma once

#include "Salad/Scene/Entity.h"
#include "EditorPanelBase.hpp"

namespace Salad::EditorGui {

	class ScenePropertiesPanel : public EditorPanelBase {

	public:
		ScenePropertiesPanel() :
			EditorPanelBase("Properties", true)
		{};

		virtual void init() override;
		virtual void loadSettings() override;
		virtual void setContext(const Ref<Scene>& scene) override {}

	protected:
		virtual void onImGuiRender() override;

	private:
		void renderEntityProperties();
		void renderEditorShaderProperties();
		void renderEditorTextureProperties();
	};
}
