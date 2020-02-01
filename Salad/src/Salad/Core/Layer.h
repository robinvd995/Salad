#pragma once

#include "Salad/Core/Core.h"
#include "Salad/Events/Event.h"
#include "Salad/Core/Timestep.h"

namespace Salad {
	class SALAD_API Layer {

	public:
		Layer(const std::string& debugname = "Layer");
		~Layer();

		virtual void onAttach() {}
		virtual void onDetach() {}
		virtual void onUpdate(Timestep ts) {}
		virtual void onImGuiRender() {};
		virtual void onEvent(Event& event) {}

		inline const std::string& getName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}