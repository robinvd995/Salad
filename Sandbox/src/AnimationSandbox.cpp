#include "AnimationSandbox.h"

AnimationSandbox::AnimationSandbox() {}

void AnimationSandbox::onAttach() {
	Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.85f, 1.0f));
}

void AnimationSandbox::onDetach() {}

void AnimationSandbox::onUpdate(Salad::Timestep ts) {

	Salad::RenderCommand::clear();

}

void AnimationSandbox::onEvent(Salad::Event& e) {};