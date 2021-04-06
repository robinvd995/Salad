#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

class GuiSandbox : public Salad::Layer {

public:

	GuiSandbox();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onEvent(Salad::Event& e) override;

private:
	bool onWindowResizedEvent(Salad::WindowResizeEvent& e);

private:

	Salad::OrthographicCamera m_Camera;

	Salad::Ref<Salad::Framebuffer> m_Framebuffer;
	Salad::Ref<Salad::GuiWindow> m_GuiWindow;

	Salad::Ref<Salad::Font> m_Font;
	Salad::Ref<Salad::VertexArray> m_Text;
};