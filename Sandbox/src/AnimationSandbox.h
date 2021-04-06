#pragma once

#include <Salad.h>
#include <Salad/PerspectiveCameraController.h>

class AnimationSandbox : public Salad::Layer {

public:

	AnimationSandbox();

	virtual void onAttach() override;
	virtual void onDetach() override;

	virtual void onUpdate(Salad::Timestep ts) override;
	virtual void onEvent(Salad::Event& e) override;

};