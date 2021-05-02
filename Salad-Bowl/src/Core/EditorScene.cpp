#include "EditorScene.h"

#include "Salad/Renderer/TextureManager.h"
#include "Io/ColladaLoader.h"
#include "EditorSelectionContext.h"
#include "EditorSceneSerializer.h";

namespace Salad {

	void EditorScene::init() {

		ColladaLoader loader;
		auto cubeVao = VertexArray::create();

		{
			auto meshes = loader.loadColladaModel("assets/models/cube.dae");
			Mesh* cubeMesh = meshes[0];

			Ref<Salad::VertexBuffer> cubeVbo = VertexBuffer::create(cubeMesh->getBuffer().getBuffer(), cubeMesh->getBuffer().getBufferSize() * 4, SLD_STATIC_DRAW);
			cubeVbo->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal",   Salad::ShaderDataType::Float3 }
				});

			cubeVao->addVertexBuffer(cubeVbo);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&cubeMesh->getIndexBuffer().front(), cubeMesh->getIndexBuffer().size());
			cubeVao->setIndexBuffer(indexBuffer);
			cubeVao->unbind();
		}

		auto treeMeshes = loader.loadColladaModel("assets/models/tree.dae");
		auto treeVao = VertexArray::create();
		{
			Mesh* treeMesh = treeMeshes[0];
			Ref<Salad::VertexBuffer> cubeVbo = VertexBuffer::create(treeMesh->getBuffer().getBuffer(), treeMesh->getBuffer().getBufferSize() * 4, SLD_STATIC_DRAW);
			cubeVbo->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal",   Salad::ShaderDataType::Float3 }
				});

			treeVao->addVertexBuffer(cubeVbo);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&treeMesh->getIndexBuffer().front(), treeMesh->getIndexBuffer().size());
			treeVao->setIndexBuffer(indexBuffer);
			treeVao->unbind();
		}

		auto treeLeavesVao = VertexArray::create();
		{
			Mesh* leavesMesh = treeMeshes[1];
			Ref<Salad::VertexBuffer> cubeVbo = VertexBuffer::create(leavesMesh->getBuffer().getBuffer(), leavesMesh->getBuffer().getBufferSize() * 4, SLD_STATIC_DRAW);
			cubeVbo->setLayout({
				{ "a_Position", Salad::ShaderDataType::Float3 },
				{ "a_TexCoord", Salad::ShaderDataType::Float2 },
				{ "a_Normal",   Salad::ShaderDataType::Float3 }
				});

			treeLeavesVao->addVertexBuffer(cubeVbo);
			Ref<IndexBuffer> indexBuffer = IndexBuffer::create(&leavesMesh->getIndexBuffer().front(), leavesMesh->getIndexBuffer().size());
			treeLeavesVao->setIndexBuffer(indexBuffer);
			treeLeavesVao->unbind();
		}

		auto diffuseShader = Shader::create("assets/shaders/Diffuse.glsl");

		//Ref<Texture2D> cubeTexture = loadTexture2D("textures.crate_diffuse");
		auto cubeTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_diffuse.png");
		auto grassTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/grass_painted_large.png");
		auto leavesTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/leaves_cartoon.png");
		auto treeBarkTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/tree_bark_cartoon.png");

		auto m_CubeEntity = createEntity("Crate");
		m_CubeEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, cubeTexture);
		//m_CubeEntity.addComponent<NativeScriptComponent>().bind<CubeController>();
		m_CubeEntity.getComponent<TransformComponent>().Transform.setPosition(0.0f, 1.0f, 0.0f);

		m_CubeEntity = createEntity("Crate2");
		m_CubeEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, cubeTexture);
		//m_CubeEntity.addComponent<NativeScriptComponent>().bind<CubeController>();
		m_CubeEntity.getComponent<TransformComponent>().Transform.setPosition(-3.0f, 1.0f, 1.5f);

		auto m_TreeEntity = createEntity("Tree");
		m_TreeEntity.addComponent<MeshComponent>(treeVao, diffuseShader, treeBarkTexture);
		m_TreeEntity.getComponent<TransformComponent>().Transform.setPosition(3.0f, 0.0f, -3.0f);
		m_TreeEntity.getComponent<TransformComponent>().Transform.setScale(0.4f, 0.4f, 0.4f);

		auto m_LeavesEntity = createEntity("Leaves");
		m_LeavesEntity.addComponent<MeshComponent>(treeLeavesVao, diffuseShader, leavesTexture);

		m_LeavesEntity.setParent(m_TreeEntity);

		auto m_PlatformEntity = createEntity("Platform");
		m_PlatformEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, grassTexture);
		m_PlatformEntity.getComponent<TransformComponent>().Transform.setPosition(0.0f, 0.005f, 0.0f);
		m_PlatformEntity.getComponent<TransformComponent>().Transform.setScale(10.0f, 0.01f, 10.0f);

		auto m_CameraEntity = createEntity("Camera");
		PerspectiveCameraProperties camProps;
		m_CameraEntity.addComponent<CameraComponent>(camProps);

		m_EditorCamera.init();
	}

	void EditorScene::cleanup() {

	}

	void EditorScene::startRuntime() {
		if(m_EditorState == EditorState::Editor) {
			// serialize scene to a temp folder

			EditorSceneSerializer serializer(this);
			serializer.serializeScene(".editor/cache/scene-snapshot.xml");

			onSceneStart();
			m_EditorState = EditorState::Runtime;
			EditorEventManager::submitEvent(EditorEventStateChanged(static_cast<int>(m_EditorState)));
		}
		else if (m_EditorState == EditorState::RuntimePaused) {
			m_EditorState = EditorState::Runtime;
			EditorEventManager::submitEvent(EditorEventStateChanged(static_cast<int>(m_EditorState)));
		}
	}

	void EditorScene::pauseRuntime() {
		if(m_EditorState == EditorState::Runtime) {
			m_EditorState = EditorState::RuntimePaused;
			EditorEventManager::submitEvent(EditorEventStateChanged(static_cast<int>(m_EditorState)));
		}
	}

	void EditorScene::stopRuntime() {
		if (m_EditorState == EditorState::Runtime || m_EditorState == EditorState::RuntimePaused) {
			onSceneEnd();

			// deserialize scene from the temp folder

			m_EditorState = EditorState::Editor;
			EditorEventManager::submitEvent(EditorEventStateChanged(static_cast<int>(m_EditorState)));
		}
	}

	void EditorScene::onUpdate(Timestep& ts) {
		if (m_ViewportHovered && m_EditorState != EditorState::Runtime) {
			m_EditorCamera.updateCamera(ts);
		}
		Scene::onUpdate(ts);
	}

	bool EditorScene::beginRenderScene() {
		if (m_EditorState == EditorState::Runtime) return Scene::beginRenderScene();
		else {
			Salad::Renderer::beginScene({ m_EditorCamera.getProjection(), m_EditorCamera.getViewMatrix() });
			return true;
		}
	}

	void EditorScene::endRenderScene() {
		Scene::endRenderScene();
	}

	void EditorScene::renderMeshes(Timestep ts) {
		if (m_EditorState == EditorState::Runtime) Scene::renderMeshes(ts);
		else {
			auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);

			Entity selectedEntity;
			if (EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
				EntitySelectionContext* selectionContext = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
				selectedEntity = selectionContext->getSelectedEntity();
			}

			for (auto entity : group) {
				auto [t, m] = group.get<TransformComponent, MeshComponent>(entity);

				m.MeshTexture->bind();
				Salad::Renderer::submitEditorMesh(m.MeshShader, m.MeshVertexArray, t.Transform.getWorldSpaceTransformationMatrix(), (int)entity, entity == selectedEntity);
			}
		}
	}

	void EditorScene::updateScripts(Timestep ts) {
		if (m_EditorState == EditorState::Runtime) Scene::updateScripts(ts);
		else {}
	}

	void EditorScene::updateTransforms(Timestep ts) {
		Scene::updateTransforms(ts);
	}

	void EditorScene::onEditorEvent(const EditorEvent& evnt) {
		switch(evnt.type) {

		}
	}
}
