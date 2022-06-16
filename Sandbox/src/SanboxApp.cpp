#include <Salad.h>
#include <Salad/Core/EntryPoint.h>

#include "OpenGL/OpenGLSanbox.hpp"
#include "Gui/GuiSandbox.hpp"
#include "Resources/ResourceLayer.h"

class SandboxInitLayer : public Salad::Layer {

public:
	SandboxInitLayer() : Layer("SandboxInitLayer") {}
	~SandboxInitLayer() = default;
	
	virtual void onAttach() override {
		Salad::RenderCommand::setClearColor(glm::vec4(1.0f, 0.01f, 0.01f, 1.0f));
	}

	virtual void onDetach() override {}

	virtual void onUpdate(Salad::Timestep ts) override {
		Salad::RenderCommand::clear();
	}

	virtual void onImGuiRender() override {}

	virtual void onEvent(Salad::Event& event) override {}
};

class Sandbox : public Salad::Application {
public:
	Sandbox() {
		pushLayer(new SandboxInitLayer());
		pushLayer(new Salad::ResourceLayer());
		pushLayer(new Salad::OpenGLSandbox());
		//pushLayer(new Salad::GuiSandbox());
	}
	~Sandbox() {}
};

Salad::Application* Salad::createApplication() {
	return new Sandbox();
}

Salad::ImGuiLayer* Salad::createImGuiLayer() {
	return new ImGuiLayer();
}

