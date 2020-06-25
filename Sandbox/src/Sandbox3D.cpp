#include "Sandbox3D.h"

#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/TextureManager.h"

Sandbox3D::Sandbox3D() :
	m_Camera(Salad::PerspectiveCameraProps())
{
	using namespace Salad;

	float minX = -0.5f, minY = -0.5f, minZ = -0.5f, minU = 0.0f, minV = 0.0f;
	float maxX =  0.5f, maxY =  0.5f, maxZ =  0.5f, maxU = 1.0f, maxV = 1.0f;

	float vertexData[] = {
		minX, minY, minZ, minU, minV, 0.0f, 0.0f, 0.0f,
		maxX, minY, minZ, maxU, minV, 0.0f, 0.0f, 0.0f,
		minX, minY, maxZ, minU, maxV, 0.0f, 0.0f, 0.0f,
		maxX, minY, maxZ, maxU, maxV, 0.0f, 0.0f, 0.0f,

		minX, maxY, minZ, minU, maxV, 0.0f, 0.0f, 0.0f,
		maxX, maxY, minZ, maxU, maxV, 0.0f, 0.0f, 0.0f,
		minX, maxY, maxZ, minU, minV, 0.0f, 0.0f, 0.0f,
		maxX, maxY, maxZ, maxU, minV, 0.0f, 0.0f, 0.0f
	};

	uint32_t indexData[] = {
		0, 1, 2, 1, 3, 2, // Bottom Face
		4, 5, 6, 5, 7, 6, // Top Face
		0, 1, 5, 0, 5, 4, // Front Face
		2, 3, 7, 2, 7, 6, // Back Face
		0, 2, 6, 0, 6, 4, // Left Face
		1, 3, 7, 1, 7, 5  // Right Face
	};

	m_Cube = VertexArray::create();

	Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(vertexData, sizeof(vertexData), SLD_STATIC_DRAW);
	vertexBuffer->setLayout({
		{ "a_Position", Salad::ShaderDataType::Float3 },
		{ "a_TexCoord", Salad::ShaderDataType::Float2 },
		{ "a_Normal",   Salad::ShaderDataType::Float3 }
	});

	m_Cube->addVertexBuffer(vertexBuffer);
	Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indexData, 36);
	m_Cube->setIndexBuffer(indexBuffer);
	m_Cube->unbind();

	m_Shader = Shader::create("assets/shaders/Static.glsl");
	
	{
		Salad::Ref<Salad::Texture2D> t0 = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_diffuse.png");
	}
}

void Sandbox3D::onAttach() {
	Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.85f, 1.0f));
}

void Sandbox3D::onDetach() {
	
}

void Sandbox3D::onUpdate(Salad::Timestep ts) {

	// Update
	m_Camera.onUpdate(ts);

	// Render
	Salad::RenderCommand::clear();

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -10.0f));

	Salad::Renderer::beginScene(m_Camera.getCamera());
	Salad::TextureManager::get().getTexture2D("assets/textures/crate_diffuse.png")->bind();
	Salad::Renderer::submit(m_Shader, m_Cube, transform);
	Salad::Renderer::endScene();
}

void Sandbox3D::onEvent(Salad::Event& e) {

}
