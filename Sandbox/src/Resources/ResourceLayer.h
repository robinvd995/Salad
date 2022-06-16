#pragma once

#include "Salad/Core/Layer.h"

namespace Salad {

	class ResourceLayer : public Layer {

	public:
		ResourceLayer();
		~ResourceLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;
		virtual void onUpdate(Timestep ts) override;
		virtual void onImGuiRender() override;
		virtual void onEvent(Event& event) override;
	};

}