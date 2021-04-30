#include "EditorLayer.h"

#include "Salad/Core/Core.h"

#include "Salad/Renderer/Tessellator.h"
#include "Salad/Renderer/TextureManager.h"
#include "Salad/Renderer/Renderer.h"

#include "Salad/Scene/SceneSerializer.h"

#include "ImGui/imgui.h"
#include "ImGuizmo.h"

#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include "EditorGui/EditorStyle.h"

#include "EditorAssetManager.h"

#include "Salad/Math/Math.h"

#include <functional>

//TEMP 
#include "Salad/Renderer/TextureLoader.hpp"

namespace Salad {

	EditorLayer::EditorLayer()
	{
		/*float minX = -0.5f, minY = -0.5f, minZ = -0.5f, minU = 0.0f, minV = 0.0f;
		float maxX = 0.5f, maxY = 0.5f, maxZ = 0.5f, maxU = 1.0f, maxV = 1.0f;

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
		};*/

		m_Scene = createRef<Scene>();

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

		/*auto cubeVao = VertexArray::create();

		Ref<Salad::VertexBuffer> cubeVbo = VertexBuffer::create(vertexData, sizeof(vertexData), SLD_STATIC_DRAW);
		cubeVbo->setLayout({
			{ "a_Position", Salad::ShaderDataType::Float3 },
			{ "a_TexCoord", Salad::ShaderDataType::Float2 },
			{ "a_Normal",   Salad::ShaderDataType::Float3 }
		});

		cubeVao->addVertexBuffer(cubeVbo);
		Ref<IndexBuffer> indexBuffer = IndexBuffer::create(indexData, 36);
		cubeVao->setIndexBuffer(indexBuffer);
		cubeVao->unbind();*/

		// Load editor textures
		//m_TextureButtonPlay = Salad::TextureManager::get().loadTexture2D("assets/textures/buttons/button_play.png");
		//m_TextureButtonPause = Salad::TextureManager::get().loadTexture2D("assets/textures/buttons/button_pause.png");
		//m_TextureButtonStop = Salad::TextureManager::get().loadTexture2D("assets/textures/buttons/button_stop.png");

		//m_EditorIcons = Salad::TextureManager::get().loadTexture2D("assets/textures/editor_icons.png");

		auto diffuseShader = Shader::create("assets/shaders/Diffuse.glsl");
		Ref<Texture2D> cubeTexture = loadTexture2D("textures.crate_diffuse");

		//auto cubeTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_diffuse.png");
		auto grassTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/grass_painted_large.png");
		auto leavesTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/leaves_cartoon.png");
		auto treeBarkTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/tree_bark_cartoon.png");

		class CubeController : public ScriptableEntity {
		
		public:
			virtual void onCreate() {}

			virtual void onUpdate(Timestep ts) {
				auto& transform = getComponent<TransformComponent>().Transform;

				float dx = 0.0f;
				float dy = 0.0f;
				if(Input::isKeyPressed(SLD_KEY_A)) {
					dx -= 5.0f * ts;
				}
				if (Input::isKeyPressed(SLD_KEY_D)) {
					dx += 5.0f * ts;
				}
				if (Input::isKeyPressed(SLD_KEY_W)) {
					dy += 5.0f * ts;
				}
				if (Input::isKeyPressed(SLD_KEY_S)) {
					dy -= 5.0f * ts;
				}

				transform.translate(dx, dy, 0.0f);
			}

			virtual void onDestroy() override {}
		
		};

		m_CubeEntity = m_Scene->createEntity("Crate");
		m_CubeEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, cubeTexture);
		m_CubeEntity.addComponent<NativeScriptComponent>().bind<CubeController>();
		m_CubeEntity.getComponent<TransformComponent>().Transform.setPosition(0.0f, 1.0f, 0.0f);

		m_CubeEntity = m_Scene->createEntity("Crate2");
		m_CubeEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, cubeTexture);
		m_CubeEntity.addComponent<NativeScriptComponent>().bind<CubeController>();
		m_CubeEntity.getComponent<TransformComponent>().Transform.setPosition(-3.0f, 1.0f, 1.5f);

		m_TreeEntity = m_Scene->createEntity("Tree");
		m_TreeEntity.addComponent<MeshComponent>(treeVao, diffuseShader, treeBarkTexture);
		m_TreeEntity.getComponent<TransformComponent>().Transform.setPosition(3.0f, 0.0f, -3.0f);
		m_TreeEntity.getComponent<TransformComponent>().Transform.setScale(0.4f, 0.4f, 0.4f);

		m_LeavesEntity = m_Scene->createEntity("Leaves");
		m_LeavesEntity.addComponent<MeshComponent>(treeLeavesVao, diffuseShader, leavesTexture);

		m_LeavesEntity.setParent(m_TreeEntity);

		m_PlatformEntity = m_Scene->createEntity("Platform");
		m_PlatformEntity.addComponent<MeshComponent>(cubeVao, diffuseShader, grassTexture);
		m_PlatformEntity.getComponent<TransformComponent>().Transform.setPosition(0.0f, 0.005f, 0.0f);
		m_PlatformEntity.getComponent<TransformComponent>().Transform.setScale(10.0f, 0.01f, 10.0f);

		m_CameraEntity = m_Scene->createEntity("Camera");
		PerspectiveCameraProperties camProps;
		m_CameraEntity.addComponent<CameraComponent>(camProps);

		float skyboxVertices[] = {
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		m_SkyboxVAO = VertexArray::create();
		Ref<Salad::VertexBuffer> skyboxVbo = VertexBuffer::create(skyboxVertices, sizeof(skyboxVertices), SLD_STATIC_DRAW);
		skyboxVbo->setLayout({
			{ "a_Position", Salad::ShaderDataType::Float3 }
			});
		m_SkyboxVAO->addVertexBuffer(skyboxVbo);
		m_SkyboxVAO->unbind();

		m_SkyboxShader = Shader::create("assets/shaders/Skybox.glsl");
		m_SkyBoxTexture = TextureCubeMap::create("assets/textures/skybox/default", ".png");

		FramebufferSpecification fbspec;
		fbspec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INTEGER, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
		fbspec.Width = 1280;
		fbspec.Height = 720;

		m_Framebuffer = Framebuffer::create(fbspec);

		//m_SceneHierarchyPanel.setContext(m_Scene);

		// Post processing setup

		// ----- PSEUDO START -----
		// 
		// PostProcessing pipeline = new PostProcessing(m_Framebuffer);
		// 
		// PostProcessingEffect gaussianblur = ppPipeline->addEffect<PostProcessingGaussianBlur>(entrypoint);
		// PostProcessingEffect bloom = ppPipeline->addEffect<PostProcessingBloomEffect>(gaussianblur);
		// PostProcessingEffect outline = ppPipeline->addEffect<PostProcessingOutlineEffect>(outline);
		// 
		// pipeline.connect();
		// 
		// ----- PSEUDO END -----

		//PostProcessing pipeline;
		//PostProcessingEffect gaussianblur = pipeline.addEffect<PostProcessingBloom>();
		{
			auto ppShader = Shader::create("assets/shaders/postprocessing/Outline.glsl");
			FramebufferSpecification ppFbSpec;
			ppFbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
			ppFbSpec.Width = 1280;
			ppFbSpec.Height = 720;
			auto ppFramebuffer = Framebuffer::create(ppFbSpec);

			m_PostProcessingOutline = PostProcessingEffect(ppShader, ppFramebuffer);
		}

		{
			auto ppShader = Shader::create("assets/shaders/postprocessing/Composer.glsl");
			FramebufferSpecification ppFbSpec;
			ppFbSpec.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
			ppFbSpec.Width = 1280;
			ppFbSpec.Height = 720;
			auto ppFramebuffer = Framebuffer::create(ppFbSpec);

			m_PostProcessingComposer = PostProcessingComposer(ppShader, ppFramebuffer);
		}
	}

	void EditorLayer::onAttach() {
		Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.85f, 1.0f));

		m_EditorUI.init();
		m_EditorUI.setSceneContext(m_Scene);

		// Settings
		EditorSettings::s_Instance = new EditorSettings();
		m_EditorUI.loadPanelSettings();
		m_EditorCamera.init();
		EditorSettings::s_Instance->deserializeSettings();

		// Asset Manager Initialization
		EditorAssetManager::instantiate("temp_resource_output.zip", "temp_asset_registry.xml");

		m_EditorUI.setViewportRenderId(m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0));
		std::function<void()> func = std::bind(&EditorLayer::onViewportResized, this);
		m_EditorUI.setViewportResizeHandler(func);
		m_EditorUI.setEditorCamera(&m_EditorCamera);

		// Create a new instance of the editor selection context
		EditorSelectionContext::s_Instance = new EditorSelectionContext();
	}

	void EditorLayer::onDetach() {
		delete EditorSelectionContext::s_Instance;
		EditorSelectionContext::s_Instance = nullptr;

		delete EditorSettings::s_Instance;
		EditorSettings::s_Instance = nullptr;

		delete EditorAssetManager::s_Instance;
		EditorAssetManager::s_Instance = nullptr;
	}

	void EditorLayer::onUpdate(Timestep ts) {

		auto curTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> ms_double = curTime - m_PrevFrameStartTime;

		m_CurFrames++;
		if(ms_double.count() >= 1000.0) {
			m_CountedFrames = m_CurFrames;
			m_CurFrames = 0;
			m_PrevFrameStartTime = curTime;
		}

		// bind frame buffer
		m_Framebuffer->bind();

		// Render
		Salad::RenderCommand::clear();

		// Very simple state changing code
		// only used to test the editor camera vs runtime camera
		// TODO: Make an actual state
		/*if(Input::isKeyPressed(SLD_KEY_R)) {
			m_EditorState = EditorState::Runtime;
		}
		else{
			m_EditorState = EditorState::Editor;
		}*/

		//skybox rendering
		m_SkyBoxTexture->bind();
		Salad::RenderCommand::depthMask(false);
		Salad::Renderer::submitSkybox(m_SkyboxShader, m_SkyboxVAO);
		Salad::RenderCommand::depthMask(true);

		int clearData = -1;
		m_Framebuffer->clearColorAttachment(1, &clearData);
		float clearColor[4] = { 0, 0, 0, 1 };
		m_Framebuffer->clearColorBuffer(2, &clearColor[0]);

		switch(m_EditorState){
			case EditorState::Editor: {
				if (m_EditorUI.isViewportFocused()) m_EditorCamera.updateCamera(ts);

				Entity entity;
				if(EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
					EntitySelectionContext* selectionContext = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
					entity = selectionContext->getSelectedEntity();
				}
				m_Scene->onUpdateEditor(ts, m_EditorCamera, m_EditorCamera.getViewMatrix(), entity);
			} break;
			case EditorState::Runtime: m_Scene->onUpdate(ts);
		}

		if(m_EditorUI.isMouseInViewport()) {
			int pixeldata = m_Framebuffer->readPixel(1, m_EditorUI.getViewportMouseX(), m_EditorUI.getViewportMouseY());
			if(pixeldata != clearData) {
				m_HoveredEntity = { (uint32_t)pixeldata, m_Scene.get() };
			}
			else {
				m_HoveredEntity = {};
			}
		}
		else{
			m_HoveredEntity = {};
		}

		// unbind frame buffer
		m_Framebuffer->unbind();

		if(m_PostProcessingFramebufferResize){
			m_PostProcessingOutline.getFramebuffer()->resize(m_PostProcessingFramebufferWidth, m_PostProcessingFramebufferHeight);
			m_PostProcessingComposer.getFramebuffer()->resize(m_PostProcessingFramebufferWidth, m_PostProcessingFramebufferHeight);
			m_PostProcessingFramebufferResize = false;
		}

		m_PostProcessingOutline.draw(m_Framebuffer);
		m_PostProcessingComposer.draw(m_Framebuffer, m_PostProcessingOutline.getFramebuffer());
	}

	void EditorLayer::onEvent(Event& e) {
		EventDispatcher dispatcher(e);
		dispatcher.dispatch<MouseScrolledEvent>(SLD_BIND_EVENT_FN(EditorLayer::onMouseScrolledEvent));
		dispatcher.dispatch<KeyPressedEvent>(SLD_BIND_EVENT_FN(EditorLayer::onKeyPressedEvent));
		dispatcher.dispatch<MouseButtonPressedEvent>(SLD_BIND_EVENT_FN(EditorLayer::onMousePressedEvent));
		if(!e.handled) m_EditorCamera.onEvent(e);
	}

	bool EditorLayer::onMouseScrolledEvent(MouseScrolledEvent& e) {
		return !m_EditorUI.isViewportHovered();
	}

	bool EditorLayer::onKeyPressedEvent(KeyPressedEvent& e) {

		if (e.getRepeatCount() > 0) return false;

		bool ctrlDown = Input::isKeyPressed(SLD_KEY_LEFT_CONTROL) || Input::isKeyPressed(SLD_KEY_RIGHT_CONTROL);
		bool shftDown = Input::isKeyPressed(SLD_KEY_LEFT_SHIFT) || Input::isKeyPressed(SLD_KEY_RIGHT_SHIFT);

		switch(e.getKeyCode()) {
		
			case SLD_KEY_Q: m_EditorUI.setGizmoType(-1); break;
			case SLD_KEY_W: m_EditorUI.setGizmoType(ImGuizmo::OPERATION::TRANSLATE); break;
			case SLD_KEY_E: m_EditorUI.setGizmoType(ImGuizmo::OPERATION::ROTATE); break;
			case SLD_KEY_R:m_EditorUI.setGizmoType(ImGuizmo::OPERATION::SCALE); break;
			case SLD_KEY_ESCAPE: EditorSelectionContext::setSelectionContextNone(); break;
			//case SLD_KEY_ESCAPE: m_EditorSelectionContext->setSelectionContext({}); break; -------------------------------------------------------------
		}
	}

	bool EditorLayer::canMousePick() {
		return m_HoveredEntity.isValid() && !m_EditorUI.isGizmoHovered();
	}

	bool EditorLayer::onMousePressedEvent(MouseButtonPressedEvent& e) {
		if (e.getMouseButton() == 0 && canMousePick()) {
			//EntitySelectionContext context(m_HoveredEntity);
			EditorSelectionContext::setSelectionContext<EntitySelectionContext>(m_HoveredEntity);
			//m_EditorSelectionContext->setSelectionContext(m_HoveredEntity); --------------------------------------------------------------
			return true;
		}

		return false;
	}

	void EditorLayer::onViewportResized() {
		uint32_t width = m_EditorUI.getViewportWidth();
		uint32_t height = m_EditorUI.getViewportHeight();

		m_Framebuffer->resize(width, height);
		m_PostProcessingFramebufferResize = true;
		m_PostProcessingFramebufferWidth = width;
		m_PostProcessingFramebufferHeight = height;

		// TODO: Let camera handle viewport resizing based on projection type
		PerspectiveCameraProperties camProps;
		camProps.AspectRatio = (float)width / (float)height;
		m_EditorCamera.setPerspectiveProjection(camProps);
		m_EditorCamera.recalculateViewMatrix();

		Renderer::onWindowResized((uint32_t)width, (uint32_t)height);
	}

	void EditorLayer::onImGuiRender() { 
		m_EditorUI.setHoveredEntity(m_HoveredEntity);
		m_EditorUI.setCurrentFrames(m_CountedFrames);
		m_EditorUI.onImGuiRender();
	}

	void EditorLayer::serialize() {
		SceneSerializer serializer(m_Scene);
		serializer.serializeScene("assets/scenes/example.sld");
	}

	void EditorLayer::deserialize() {}

}
