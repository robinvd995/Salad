#pragma once

#include "Salad/Core/Core.h"
#include "Layer.h"

#include <vector>

namespace Salad {

	typedef std::vector<Layer*> LStack;
	typedef std::vector<Layer*>::iterator LStackIt;

	class SALAD_API LayerStack{
	public:

		LayerStack();
		~LayerStack();

		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);
		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		LStackIt begin() { return m_Layers.begin(); }
		LStackIt end() { return m_Layers.end(); }

	private:
		LStack m_Layers;
		unsigned int m_LayerInsertIndex = 0;
	};
}