#pragma once

#include "Salad/Core/Layer.h"
#include "Salad/Events/ApplicationEvent.h"
#include "Salad/Events/KeyEvent.h"
#include "Salad/Events/MouseEvent.h"

namespace Salad {

	class SALAD_API ImGuiLayer : public Layer {

	public:
		ImGuiLayer();
		~ImGuiLayer();

		void begin();
		void end();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onImGuiRender() override;
	private:
		float m_Time = 0.0f;
	};

}