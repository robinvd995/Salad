#pragma once

#include "imgui/imgui.h"
#include "Salad/Scene/Scene.h"

#include <string>

namespace Salad::EditorGui {

	class EditorPanelBase {
	
	public:
		EditorPanelBase(const std::string& panelName, bool defaultOpen = true) :
			m_PanelName(panelName), m_PanelOpen(defaultOpen)
		{}

		void renderPanel() {
			if (!m_PanelOpen) return;
			ImGui::Begin(m_PanelName.c_str());
			onImGuiRender();
			ImGui::End();
		}

		virtual void init() {};
		virtual void loadSettings() {};

		virtual void setContext(const Ref<Scene>& scene) {}

		virtual const std::string getPanelName() { return m_PanelName; }
		virtual bool* getPanelStatus() { return &m_PanelOpen; }

	protected:
		virtual void onImGuiRender() = 0;

	private:
		const std::string m_PanelName;
		bool m_PanelOpen = false;
	};
}
