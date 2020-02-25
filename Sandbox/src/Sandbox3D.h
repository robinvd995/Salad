#pragma once

#include <Salad.h>
#include <Salad/OrthographicCameraController.h>
#include <Salad/Renderer/Gui/FontManager.h>

class Sandbox3D : public Salad::Layer {

public:

	Sandbox3D();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onEvent(Salad::Event& e) override;

private:

};