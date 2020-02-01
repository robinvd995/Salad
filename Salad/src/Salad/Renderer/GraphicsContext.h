#pragma once

#include "Salad/Core/Core.h"

namespace SALAD_API Salad {

	class GraphicsContext {

	public:
		virtual void init() = 0;
		virtual void swapBuffers() = 0;
	};

}