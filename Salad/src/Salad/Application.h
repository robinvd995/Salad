#pragma once

#include "Core.h"

namespace Salad {

	class SALAD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	//Define in client
	Application* createApplication();
}
