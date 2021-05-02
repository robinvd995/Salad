#pragma once

#include "Salad/Core/Layer.h"
#include "Salad/Events/ApplicationEvent.h"
#include "Salad/Events/KeyEvent.h"
#include "Salad/Events/MouseEvent.h"

namespace Salad {

	class ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		ImGuiLayer(const char* imGuiIniFile);
		~ImGuiLayer();

		void begin();
		void end();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& e) override;

	private:
		float m_Time = 0.0f;
		bool m_HasCustomIniLocation = false;
		const char* m_ImGuiIniFile;
	};

}
