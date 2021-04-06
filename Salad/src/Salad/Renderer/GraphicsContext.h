#pragma once

#include "Salad/Core/Core.h"

namespace Salad {

	class GraphicsContext {

	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};

}