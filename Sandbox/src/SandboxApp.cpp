#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "Sandbox2D.h"
#include "Sandbox3D.h"
#include "LightingSandbox.h"
#include "GuiLayer.h"
#include "GuiSandbox.h"
#include "AnimationSandbox.h"

class Sandbox : public Salad::Application {
public:
	Sandbox() {
		//pushLayer(new Sandbox2D());
		pushLayer(new Sandbox3D());
		//pushLayer(new AnimationSandbox());
		//pushLayer(new LightingSandbox());
		//pushOverlay(new GuiSandbox());
		//pushLayer(new GuiLayer());
		//pushLayer(new SkeletalAnimationSandbox());
	}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}
