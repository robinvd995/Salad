#pragma once

#ifdef SLD_PLATFORM_WINDOWS

extern Salad::Application* Salad::createApplication();

int main(int argcm, char** argv) {
	auto app = Salad::createApplication();
	app->run();
	delete app;
}

#endif