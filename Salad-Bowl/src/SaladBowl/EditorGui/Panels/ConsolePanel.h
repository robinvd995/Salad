#pragma once

#include "EditorPanelBase.hpp"

namespace Salad::EditorGui {

	class ConsolePanel : public EditorPanelBase {

	public:
		ConsolePanel() :
			EditorPanelBase("Console", true) 
		{}
		~ConsolePanel() = default;

		virtual void init() {};
		virtual void loadSettings() {};
		virtual void setContext(const Ref<Scene>& scene) {}

	protected:
		virtual void onImGuiRender() override;

	};
}
