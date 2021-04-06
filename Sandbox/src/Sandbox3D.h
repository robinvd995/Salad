#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

class Sandbox3D : public Salad::Layer {

public:

	Sandbox3D();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onEvent(Salad::Event& e) override;

private:

	Salad::PerspectiveCameraController m_Camera;

	Salad::Ref<Salad::Shader> m_Shader;
	Salad::Ref<Salad::VertexArray> m_Cube;

	Salad::Ref<Salad::VertexArray> m_SkyboxVAO;
	Salad::Ref<Salad::Shader> m_SkyboxShader;
	Salad::Ref<Salad::TextureCubeMap> m_SkyBoxTexture;

};