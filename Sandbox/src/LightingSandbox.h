#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

class LightingSandbox : public Salad::Layer {

public:

	LightingSandbox();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onEvent(Salad::Event& e) override;

private:

	Salad::PerspectiveCameraController m_Camera;

	Salad::Ref<Salad::Shader> m_StaticPhongShader;
	Salad::Ref<Salad::Shader> m_TexturedPhongShader;
	Salad::Ref<Salad::Shader> m_StaticShader;

	Salad::Ref<Salad::VertexArray> m_Cube;

	Salad::Ref<Salad::Entity> m_Player;
};