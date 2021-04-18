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

#include "Salad/Math/Math.h"

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
		//m_EditorSelectionContext = createRef<EditorSelectionContext>();

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

		m_EditorIcons = Salad::TextureManager::get().loadTexture2D("assets/textures/editor_icons.png");

		auto diffuseShader = Shader::create("assets/shaders/Diffuse.glsl");

		auto cubeTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/crate_diffuse.png");
		auto grassTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/grass_painted_large.png");
		auto leavesTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/leaves_cartoon.png");
		auto treeBarkTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/tree_bark_cartoon.png");

		m_MaterialTexture = Salad::TextureManager::get().loadTexture2D("assets/textures/checkerboard.png");

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

		m_SceneHierarchyPanel.setContext(m_Scene);

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

		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiIO& io = ImGui::GetIO();
		// Styling
		io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/OpenSans-Regular.ttf", 18.0f);
		io.Fonts->AddFontFromFileTTF("assets/fonts/open_sans/OpenSans-Bold.ttf", 18.0f);

		// TODO: Fix the system to where you can push and id or "section" to have the same labels available

		// Settings
		EditorCameraControlSettings& cameraSettings = m_EditorCamera.getCameraControlSettings();
		m_EditorSettingsWindow.createGroup("Editor Camera");

		// Freeflight settings
		m_EditorSettingsWindow.pushTag("Editor Camera", "Freeflight Mode");
		m_EditorSettingsWindow.pushID("Editor Camera", "FreeflightMode");
		m_EditorSettingsWindow.pushSeperator("Editor Camera");

		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Float, "FF Sensitivity", &cameraSettings.ff_Sensitivity);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "FF Invert Horizontal", &cameraSettings.ff_InvertHorizontal);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "FF Invert Vertical", &cameraSettings.ff_InvertVertical);

		m_EditorSettingsWindow.pushSeperator("Editor Camera");
		m_EditorSettingsWindow.popID("Editor Camera");

		// Orbit settings
		m_EditorSettingsWindow.pushTag("Editor Camera", "Orbit Mode");
		m_EditorSettingsWindow.pushID("Editor Camera", "OrbitMode");
		m_EditorSettingsWindow.pushSeperator("Editor Camera");

		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Float, "OR Sensitivity", &cameraSettings.or_Sensitivity);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "OR Invert Horizontal", &cameraSettings.or_InvertHorizontal);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "OR Invert Vertical", &cameraSettings.or_InvertVertical);

		m_EditorSettingsWindow.pushSeperator("Editor Camera");
		m_EditorSettingsWindow.popID("Editor Camera");

		// Scroll settings
		m_EditorSettingsWindow.pushTag("Editor Camera", "Translation Mode");
		m_EditorSettingsWindow.pushID("Editor Camera", "TranslateMode");
		m_EditorSettingsWindow.pushSeperator("Editor Camera");

		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Float, "TR Sensitivity", &cameraSettings.sc_Sensitivity);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "TR Invert Horizontal", &cameraSettings.sc_InvertHorizontal);
		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Flag, "TR Invert Vertical", &cameraSettings.sc_InvertVertical);

		m_EditorSettingsWindow.pushSeperator("Editor Camera");
		m_EditorSettingsWindow.popID("Editor Camera");

		// General Settings
		m_EditorSettingsWindow.pushTag("Editor Camera", "General");
		m_EditorSettingsWindow.pushID("Editor Camera", "General");
		m_EditorSettingsWindow.pushSeperator("Editor Camera");

		m_EditorSettingsWindow.pushProperty("Editor Camera", PropertyType::Float, "Max Camera Angle", &cameraSettings.maxCameraAngle);

		m_EditorSettingsWindow.popID("Editor Camera");

		m_FileExplorerPanel.init();

		EditorSelectionContext::s_Instance = new EditorSelectionContext();
	}

	void EditorLayer::onDetach() {
		delete EditorSelectionContext::s_Instance;
		EditorSelectionContext::s_Instance = nullptr;
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
				if (m_IsViewportFocused) m_EditorCamera.updateCamera(ts);

				Entity entity;
				if(EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
					EntitySelectionContext* selectionContext = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
					entity = selectionContext->getSelectedEntity();
				}
				m_Scene->onUpdateEditor(ts, m_EditorCamera, m_EditorCamera.getViewMatrix(), entity);
			} break;
			case EditorState::Runtime: m_Scene->onUpdate(ts);
		}

		if(m_ViewportMouseX >= 0.0f && m_ViewportMouseY >= 0.0f && m_ViewportMouseX < m_ViewportWidth && m_ViewportMouseY < m_ViewportHeight) {
			int pixeldata = m_Framebuffer->readPixel(1, m_ViewportMouseX, m_ViewportMouseY);
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
		if(!e.handled)
			m_EditorCamera.onEvent(e);
	}

	bool EditorLayer::onMouseScrolledEvent(MouseScrolledEvent& e) {
		return !m_IsViewportHovered;
	}

	bool EditorLayer::onKeyPressedEvent(KeyPressedEvent& e) {

		if (e.getRepeatCount() > 0) return false;

		bool ctrlDown = Input::isKeyPressed(SLD_KEY_LEFT_CONTROL) || Input::isKeyPressed(SLD_KEY_RIGHT_CONTROL);
		bool shftDown = Input::isKeyPressed(SLD_KEY_LEFT_SHIFT) || Input::isKeyPressed(SLD_KEY_RIGHT_SHIFT);

		switch(e.getKeyCode()) {
		
			case SLD_KEY_Q: m_GizmoType = -1; break;
			case SLD_KEY_W: m_GizmoType = ImGuizmo::OPERATION::TRANSLATE; break;
			case SLD_KEY_E: m_GizmoType = ImGuizmo::OPERATION::ROTATE; break;
			case SLD_KEY_R: m_GizmoType = ImGuizmo::OPERATION::SCALE; break;
			case SLD_KEY_ESCAPE: EditorSelectionContext::setSelectionContextNone(); break;
			//case SLD_KEY_ESCAPE: m_EditorSelectionContext->setSelectionContext({}); break; -------------------------------------------------------------
		}
	}

	bool EditorLayer::canMousePick() {
		return m_HoveredEntity.isValid() && !m_ImGuizmoIsHovering;
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

	void EditorLayer::onImGuiRender() { 
		static bool p_open = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)     {
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)     {
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu("File")) {

				if (ImGui::MenuItem("New Scene", "Ctrl+N")) { }
				if (ImGui::MenuItem("Save Scene", "Ctrl+S")) { serialize(); }
				if (ImGui::MenuItem("Open Scene", "Ctrl+O")) { deserialize(); }
				ImGui::Separator();
				if (ImGui::MenuItem("Exit", "Alt+F4")) { Application::get().close(); }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit")) {
				if (ImGui::MenuItem("Editor settings")) { m_EditorSettingsWindow.showWindow() = true; }

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View")) {

				ImGui::MenuItem("Scene Hierarchy", NULL, &m_ShowSceneHierarchyPanel);
				ImGui::MenuItem("Properties", NULL, &m_ShowScenePropertiesPanel);
				ImGui::MenuItem("Material Explorer", NULL, &m_ShowMaterialExplorerPanel);
				ImGui::MenuItem("File Explorer", NULL, &m_ShowFileExplorerPanel);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		if(m_ShowSceneHierarchyPanel) m_SceneHierarchyPanel.onImGuiRender();
		if(m_ShowScenePropertiesPanel) m_ScenePropertiesPanel.onImGuiRender();
		if(m_ShowMaterialExplorerPanel) m_MaterialExplorerPanel.onImGuiRender(m_MaterialTexture->getRendererId());
		if (m_ShowFileExplorerPanel) m_FileExplorerPanel.onImGuiRender(m_EditorIcons->getRendererId());

		ImGui::Begin("Runtime");
		ImVec2 runtimeButtonSize = {64.0f, 64.0f};
		ImGui::ImageButton((void*)m_EditorIcons->getRendererId(), ImVec2{ 32.0f, 32.0f }, ImVec2(0.125f, 0.0f), ImVec2(0.25f, 0.125f), 8);
		ImGui::SameLine();
		ImGui::ImageButton((void*)m_EditorIcons->getRendererId(), ImVec2{ 32.0f, 32.0f }, ImVec2(0.25f, 0.0f), ImVec2(0.375f, 0.125f), 8);
		ImGui::SameLine();
		ImGui::ImageButton((void*)m_EditorIcons->getRendererId(), ImVec2{ 32.0f, 32.0f }, ImVec2(0.0f, 0.0f), ImVec2(0.125f, 0.125f), 8);
		ImGui::End();

		ImGui::Begin("Info");

		ImGui::Text("Hovered Entity: ");
		ImGui::SameLine();
		if(m_HoveredEntity){
			TagComponent tag = m_HoveredEntity.getComponent<TagComponent>();
			ImGui::Text(tag.Tag.c_str());
		}
		else {
			ImGui::Text("None");
		}

		ImGui::Text("Gizmo Type: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_GizmoType).c_str());

		ImGui::Text("FPS: ");
		ImGui::SameLine();
		ImGui::Text(std::to_string(m_CountedFrames).c_str());

		ImGui::End(); // Settings end

		/*ImGui::Begin("Postprocessing test");
		ImGui::Text("Postprocessing: ");
		//uint32_t ppRenderId = m_PostProcessingOutline.getFramebuffer()->getColorAttachment(0);
		uint32_t ppRenderId = m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0);
		ImGui::Image((void*)ppRenderId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });
		ImGui::End();*/
		bool sameline = true;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");

		auto mousePos = ImGui::GetMousePos();
		auto windowPos = ImGui::GetWindowPos();
		auto windowSize = ImGui::GetWindowSize();
		auto contentRegion = ImGui::GetContentRegionAvail();

		float horizontalWaste = windowSize.x - contentRegion.x;
		float verticalWaste = windowSize.y - contentRegion.y;

		int normalizeMousePosX = mousePos.x - windowPos.x - horizontalWaste;
		int normalizeMousePosY = mousePos.y - windowPos.y - verticalWaste;

		m_ViewportMouseX = normalizeMousePosX;
		m_ViewportMouseY = contentRegion.y - normalizeMousePosY - 1;

		m_ViewportWidth = contentRegion.x;
		m_ViewportHeight = contentRegion.y;

		m_IsViewportFocused = ImGui::IsWindowFocused();
		m_IsViewportHovered = ImGui::IsWindowHovered();

		Application::variables().Flag_ImGuiBlockEvents = (!m_IsViewportFocused && !m_IsViewportHovered);

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if(m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
			SLD_CORE_INFO("Viewport size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
			uint32_t viewportSizeX = (uint32_t)viewportPanelSize.x;
			uint32_t viewportSizeY = (uint32_t)viewportPanelSize.y;

			m_Framebuffer->resize(viewportSizeX, viewportSizeY);
			m_PostProcessingFramebufferResize = true;
			m_PostProcessingFramebufferWidth = viewportSizeX;
			m_PostProcessingFramebufferHeight = viewportSizeY;

			// TODO: Let camera handle viewport resizing based on projection type
			PerspectiveCameraProperties camProps;
			camProps.AspectRatio = viewportPanelSize.x / viewportPanelSize.y;
			m_EditorCamera.setPerspectiveProjection(camProps);
			m_EditorCamera.recalculateViewMatrix();

			Renderer::onWindowResized((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
		}

		uint32_t viewportFramebufferRenderId = m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0);
		ImGui::Image((void*)viewportFramebufferRenderId, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1, 0 });

		//m_GizmoType = ImGuizmo::OPERATION::ROTATE;

		// Gizmo
		if(EditorSelectionContext::isSelectionContextType(EditorSelectionContextType::Entity)) {
			EntitySelectionContext* context = EditorSelectionContext::getSelectionContext<EntitySelectionContext>();
			Entity selectedEntity = context->getSelectedEntity(); // TODO: central selected entity
			if (selectedEntity && m_GizmoType != -1) {
				ImGuizmo::SetOrthographic(false);
				ImGuizmo::SetDrawlist();

				float windowWidth = (float)ImGui::GetWindowWidth();
				float windowHeight = (float)ImGui::GetWindowHeight();
				ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

				glm::mat4 viewMatrix = m_EditorCamera.getViewMatrix();
				glm::mat4 projectionMatrix = m_EditorCamera.getProjection();

				EntityTransform& et = selectedEntity.getComponent<TransformComponent>().Transform;
				glm::mat4 ettm = et.getWorldSpaceTransformationMatrix();

				ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(ettm));

				if (ImGuizmo::IsUsing()) {
					glm::vec3 translation{ 0.0f }, scale{ 1.0f };
					glm::quat orientation{};

					Math::decomposeTransform(ettm, translation, orientation, scale);
					et.set(translation, orientation, scale);
				}
			}
		}

		m_ImGuizmoIsHovering = ImGuizmo::IsOver();

		ImGui::End(); // Viewport end
		ImGui::PopStyleVar();
		
		ImGui::End(); // Docking end

		if(m_EditorSettingsWindow.showWindow()) {
			ImGui::Begin("Editor Settings", &m_EditorSettingsWindow.showWindow());

			m_EditorSettingsWindow.onImGuiRender();

			ImGui::End();
		}

	}

	void EditorLayer::serialize() {
		SceneSerializer serializer(m_Scene);
		serializer.serializeScene("assets/scenes/example.sld");
	}

	void EditorLayer::deserialize() {}

}
