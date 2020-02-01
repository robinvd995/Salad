#pragma once
#include "Salad/Debug/Profiler.h"

#ifdef SLD_PLATFORM_WINDOWS

extern Salad::Application* Salad::createApplication();

int main(int argcm, char** argv) {

	Salad::Log::init();
	//SLD_PROFILER_BEGIN("Startup", "SaladProfile-Startup.json");
	auto app = Salad::createApplication();
	//SLD_PROFILER_END();

	//SLD_PROFILER_BEGIN("Runtime", "SaladProfile-Runtime.json");
	app->run();
	//SLD_PROFILER_END();

	//SLD_PROFILER_BEGIN("Shutdown", "SaladProfile-Shutdown.json");
	delete app;
	//SLD_PROFILER_END();
}

#endif