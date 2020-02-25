#include "Sandbox3D.h"

Sandbox3D::Sandbox3D() {

}

void Sandbox3D::onAttach() {

}

void Sandbox3D::onDetach() {

}

void Sandbox3D::onUpdate(Salad::Timestep ts) {
	Salad::RenderCommand::setClearColor(glm::vec4(1.0f, 0.05f, 0.05f, 1.0f));
	Salad::RenderCommand::clear();
}

void Sandbox3D::onEvent(Salad::Event& e) {

}
