#include "LightingSandbox.h"

#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/TextureManager.h"
#include "Controllers/PlayerController.h"

LightingSandbox::LightingSandbox() :
	m_Camera(Salad::PerspectiveCameraProps()) {
	using namespace Salad;

	float minPos = -0.5f, maxPos = 0.5f;
	float minUV = 0.0f, maxUV = 1.0f;

	float vertexData[] = {
		minPos, minPos, minPos, minUV, minUV, 0.0f, 0.0f, -1.0f, // Front facing p0
		maxPos, minPos, minPos, maxUV, minUV, 0.0f, 0.0f, -1.0f, // Front facing p1
		maxPos, maxPos, minPos, maxUV, maxUV, 0.0f, 0.0f, -1.0f, // Front facing p2
		minPos, maxPos, minPos, minUV, maxUV, 0.0f, 0.0f, -1.0f, // Front facing p3

		minPos, minPos, maxPos, maxUV, minUV, 0.0f, 0.0f, 1.0f, // Back facing p4
		maxPos, minPos, maxPos, minUV, minUV, 0.0f, 0.0f, 1.0f, // Back facing p5
		maxPos, maxPos, maxPos, minUV, maxUV, 0.0f, 0.0f, 1.0f, // Back facing p6
		minPos, maxPos, maxPos, maxUV, maxUV, 0.0f, 0.0f, 1.0f, // Back facing p7

		maxPos, minPos, minPos, minUV, minUV, 1.0f, 0.0f, 0.0f, // Right facing p1
		maxPos, minPos, maxPos, maxUV, minUV, 1.0f, 0.0f, 0.0f, // Right facing p5
		maxPos, maxPos, maxPos, maxUV, maxUV, 1.0f, 0.0f, 0.0f, // Right facing p6
		maxPos, maxPos, minPos, minUV, maxUV, 1.0f, 0.0f, 0.0f, // Right facing p2

		minPos, minPos, minPos, maxUV, minUV, -1.0f, 0.0f, 0.0f, // Left facing p0
		minPos, maxPos, minPos, maxUV, maxUV, -1.0f, 0.0f, 0.0f, // Left facing p3
		minPos, maxPos, maxPos, minUV, maxUV, -1.0f, 0.0f, 0.0f, // Left facing p7
		minPos, minPos, maxPos, minUV, minUV, -1.0f, 0.0f, 0.0f, // Left facing p4

		minPos, minPos, minPos, minUV, minUV, 0.0f, -1.0f, 0.0f, // Bottom facing p0
		minPos, minPos, maxPos, maxUV, minUV, 0.0f, -1.0f, 0.0f, // Bottom facing p4
		maxPos, minPos, maxPos, maxUV, maxUV, 0.0f, -1.0f, 0.0f, // Bottom facing p5
		maxPos, minPos, minPos, minUV, maxUV, 0.0f, -1.0f, 0.0f, // Bottom facing p1

		minPos, maxPos, minPos, maxUV, minUV, 0.0f, 1.0f, 0.0f, // Top facing p3
		maxPos, maxPos, minPos, maxUV, maxUV, 0.0f, 1.0f, 0.0f, // Top facing p2
		maxPos, maxPos, maxPos, minUV, maxUV, 0.0f, 1.0f, 0.0f, // Top facing p6
		minPos, maxPos, maxPos, minUV, minUV, 0.0f, 1.0f, 0.0f, // Top facing p7
	};

	uint32_t indexData[] = {
		0, 1, 2, 0, 2, 3, // Front Face
		7, 6, 5, 5, 4, 7, // Back face
		8, 9, 10, 8, 10, 11, // Right face
		12, 13, 14, 12, 14, 15, // Left face
		16, 17, 18, 16, 18, 19, // Bottom face
		20, 21, 22, 20, 22, 23  // Top face
	};

	m_Cube = VertexArray::create();

	Ref<Salad::VertexBuffer> vertexBuffer = VertexBuffer::create(vertexData, sizeof(vertexData), SLD_STATIC_DRAW);
	vertexBuffer->setLayout({
		{ "a_Position", Salad::ShaderDataType::Float3 },
		{ "a_TexCoord", Salad::ShaderDataType::Float2 },
		{ "a_Normal",   Salad::ShaderDataType::Float3 }
	});

	m_Cube->addVertexBuffer(vertexBuffer);
	Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indexData, sizeof(indexData));
	m_Cube->setIndexBuffer(indexBuffer);
	m_Cube->unbind();

	m_StaticPhongShader = Shader::create("assets/shaders/StaticPhong.glsl");
	m_StaticPhongShader->bind();
	m_StaticPhongShader->setFloat3("u_Light.diffuse", glm::vec3(1.0f));
	m_StaticPhongShader->setFloat3("u_Light.ambient", glm::vec3(0.4f));
	m_StaticPhongShader->setFloat3("u_Light.specular", glm::vec3(1.0f));
	m_StaticPhongShader->unbind();

	m_TexturedPhongShader = Shader::create("assets/shaders/TexturedPhong.glsl");
	m_TexturedPhongShader->bind();
	m_TexturedPhongShader->setInt("u_Material.diffuse", 0);
	m_TexturedPhongShader->setInt("u_Material.specular", 1);
	m_TexturedPhongShader->setFloat3("u_Light.diffuse", glm::vec3(1.0f));
	m_TexturedPhongShader->setFloat3("u_Light.ambient", glm::vec3(0.4f));
	m_TexturedPhongShader->setFloat3("u_Light.specular", glm::vec3(1.0f));
	m_TexturedPhongShader->unbind();

	m_StaticShader = Shader::create("assets/shaders/Static.glsl");
	m_StaticShader->bind();
	m_StaticShader->setFloat3("u_StaticColor", glm::vec3(1.0f, 1.0f, 1.0f));
	m_StaticShader->unbind();

	{
		Salad::Ref<Salad::Texture2D> t0 = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_diffuse.png");
		Salad::Ref<Salad::Texture2D> t1 = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_specular.png");
	}

	m_Player = Salad::createRef<Salad::Entity>();
	Salad::Ref<PlayerController> playerBehaviour = Salad::createRef<PlayerController>();
	Salad::Entity::attachComponent(m_Player, Salad::createRef<Salad::EntityComponentTransform>());
	m_Player->getComponent<Salad::EntityComponentTransform>()->getTransform()->translate(glm::vec3(1.0f, 3.0f, -5.0f));
	m_Player->getComponent<Salad::EntityComponentTransform>()->getTransform()->setScale(glm::vec3(0.5f, 0.5f, 0.5f));
	Salad::Entity::attachComponent(m_Player, Salad::createRef<Salad::EntityComponentBehaviour>(playerBehaviour));
}

void LightingSandbox::onAttach() {
	Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
}

void LightingSandbox::onDetach() {

}

void LightingSandbox::onUpdate(Salad::Timestep ts) {

	// Update
	m_Player->onUpdate(ts);
	m_Camera.onUpdate(ts);

	// Render
	Salad::RenderCommand::clear();

	Salad::Ref<Salad::Transform> playerTransform = m_Player->getComponent<Salad::EntityComponentTransform>()->getTransform();

	Salad::Renderer::beginScene(m_Camera.getCamera());
	Salad::Renderer::submit(m_StaticShader, m_Cube, playerTransform->getTransformationMatrix());

	m_StaticPhongShader->bind();
	m_StaticPhongShader->setFloat3("u_Light.position", playerTransform->getPosition());
	m_StaticPhongShader->setFloat3("u_ViewPos", m_Camera.getCameraPosition());

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -10.0f));
	m_StaticPhongShader->setFloat3("u_Material.ambient", glm::vec3(0.0215f, 0.1745f, 0.0215f));
	m_StaticPhongShader->setFloat3("u_Material.diffuse", glm::vec3(0.07568f, 0.61424f, 0.07568f));
	m_StaticPhongShader->setFloat3("u_Material.specular", glm::vec3(0.633f, 0.727811f, 0.633f));
	m_StaticPhongShader->setFloat("u_Material.shininess", 76.8f);
	Salad::Renderer::submit(m_StaticPhongShader, m_Cube, transform);

	transform = glm::translate(transform, glm::vec3(2.0f, 0.0f, 0.0f));
	m_StaticPhongShader->setFloat3("u_Material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
	m_StaticPhongShader->setFloat3("u_Material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
	m_StaticPhongShader->setFloat3("u_Material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065));
	m_StaticPhongShader->setFloat("u_Material.shininess", 51.2f);
	Salad::Renderer::submit(m_StaticPhongShader, m_Cube, transform);

	transform = glm::translate(transform, glm::vec3(0.0f, 2.0f, 0.0f));
	m_StaticPhongShader->setFloat3("u_Material.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
	m_StaticPhongShader->setFloat3("u_Material.diffuse", glm::vec3(0.5f, 0.0f, 0.0f));
	m_StaticPhongShader->setFloat3("u_Material.specular", glm::vec3(0.7f, 0.6f, 0.6f));
	m_StaticPhongShader->setFloat("u_Material.shininess", 32.0f);
	Salad::Renderer::submit(m_StaticPhongShader, m_Cube, transform);

	transform = glm::translate(transform, glm::vec3(-2.0f, 0.0f, 0.0f));
	m_StaticPhongShader->setFloat3("u_Material.ambient", glm::vec3(0.25f, 0.20725f, 0.20725f));
	m_StaticPhongShader->setFloat3("u_Material.diffuse", glm::vec3(1.0f, 0.829f, 0.829f));
	m_StaticPhongShader->setFloat3("u_Material.specular", glm::vec3(0.296648f, 0.296648f, 0.296648f));
	m_StaticPhongShader->setFloat("u_Material.shininess", 112.64f);
	Salad::Renderer::submit(m_StaticPhongShader, m_Cube, transform);

	m_StaticPhongShader->unbind();

	m_TexturedPhongShader->bind();

	m_TexturedPhongShader->setFloat3("u_Light.position", playerTransform->getPosition());
	m_TexturedPhongShader->setFloat3("u_ViewPos", m_Camera.getCameraPosition());

	Salad::TextureManager::get().getTexture2D("assets/textures/crate_diffuse.png")->bind(0);
	Salad::TextureManager::get().getTexture2D("assets/textures/crate_specular.png")->bind(1);
	transform = glm::translate(transform, glm::vec3(-2.0f, 0.0f, 0.0f));
	m_TexturedPhongShader->setFloat("u_Material.shininess", 112.64f);
	Salad::Renderer::submit(m_TexturedPhongShader, m_Cube, transform);

	m_TexturedPhongShader->unbind();

	Salad::Renderer::endScene();
	
}

void LightingSandbox::onEvent(Salad::Event& e) {
	m_Camera.onEvent(e);
}
