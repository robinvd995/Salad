#pragma once

#include "Layer.h"

namespace Salad {

	class ResourceLayer : public Layer {

	public:
		ResourceLayer();

		virtual void onAttach() override;
		virtual void onDetach() override;

		virtual void onUpdate(Timestep ts) override;
		virtual void onEvent(Event& e) override;

		virtual void onImGuiRender() override;

	};
}
