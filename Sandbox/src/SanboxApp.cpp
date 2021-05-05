#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "OpenGL/OpenGLSanbox.hpp"

class Sandbox : public Salad::Application {
public:
	Sandbox() {
		pushLayer(new Salad::OpenGLSandbox());
	}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}

Salad::ImGuiLayer* Salad::createImGuiLayer() {
	return new ImGuiLayer();
}

