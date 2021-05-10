#pragma once

#include "Salad/Scene/Entity.h"

#include "EditorPanelBase.hpp"
#include "SaladBowl/EditorGui/Popups.h"

namespace Salad::EditorGui {

	class ScenePropertiesPanel : public EditorPanelBase {

	public:
		ScenePropertiesPanel() :
			EditorPanelBase("Properties", true)
		{};

		virtual void init() override;
		virtual void loadSettings() override;
		virtual void setContext(EditorScene* scene) override {}

	protected:
		virtual void onImGuiRender() override;

	private:
		void renderEntityProperties();
		void renderEditorShaderProperties();
		void renderEditorTextureProperties();
		void renderEditorModelProperties();
		void renderEditorMaterialProperties();

	private:
		AssetExplorerPopup m_AssetExplorerPopup;
	};
}
