#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "GuiLayer.h"

class Sandbox : public Salad::Application {
public:
	Sandbox() {
		pushLayer(new Sandbox3D());
		pushOverlay(new GuiLayer());
	}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}
