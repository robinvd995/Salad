#pragma once

#include "EditorPanelBase.hpp"

namespace Salad::EditorGui {

	class LogPanel : public EditorPanelBase {
	
	public:
		LogPanel() :
			EditorPanelBase("Log", true)
		{}
		~LogPanel() = default;

		virtual void init() {};
		virtual void loadSettings() {};
		virtual void setContext(EditorScene* scene) {}

	protected:
		virtual void onImGuiRender() override;

	};
}
