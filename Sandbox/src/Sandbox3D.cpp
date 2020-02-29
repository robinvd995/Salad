#include "Sandbox3D.h"

#include "Salad/Renderer/Tessellator.h"

Sandbox3D::Sandbox3D() {

	using namespace Salad;

	Tessellator::start(TessellatorMode::TRIANGLES);

	float minX = -0.5f, minY = -0.5f, minZ = -0.5f;
	float maxX =  0.5f, maxY =  0.5f, maxZ =  0.5f;

	Tessellator::pushVertex(minX, minY, minZ, 0.0f, 0.0f);
	Tessellator::pushVertex(maxX, minY, minZ, 1.0f, 0.0f);
	Tessellator::pushVertex(minX, maxY, minZ, 0.0f, 1.0f);
	Tessellator::pushVertex(maxX, maxY, minZ, 1.0f, 1.0f);

	Tessellator::pushVertex(minX, minY, maxZ, 0.0f, 0.0f);
	Tessellator::pushVertex(maxX, minY, maxZ, 1.0f, 0.0f);
	Tessellator::pushVertex(minX, maxY, maxZ, 0.0f, 1.0f);
	Tessellator::pushVertex(maxX, maxY, maxZ, 1.0f, 1.0f);

	Tessellator::end();

}

void Sandbox3D::onAttach() {

}

void Sandbox3D::onDetach() {

}

void Sandbox3D::onUpdate(Salad::Timestep ts) {
	Salad::RenderCommand::setClearColor(glm::vec4(1.0f, 0.05f, 0.05f, 1.0f));
	Salad::RenderCommand::clear();

	//Salad::Renderer::beginScene();
}

void Sandbox3D::onEvent(Salad::Event& e) {

}
