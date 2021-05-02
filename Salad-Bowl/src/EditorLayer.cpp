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

#include "Core/Core.h"

#include "EditorGui/EditorStyle.h"

#include "EditorAssetManager.h"

#include "Salad/Math/Math.h"

#include "Core/EditorEventManager.hpp"

#include <functional>

//TEMP 
#include "Salad/Renderer/TextureLoader.hpp"

namespace Salad {

	EditorLayer::EditorLayer() {
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

		// Settings
		EditorSettings::s_Instance = new EditorSettings();
		m_EditorUI.loadPanelSettings();

		m_EditorScene.init();

		m_EditorUI.init();
		m_EditorUI.setContext(&m_EditorScene);

		// Asset Manager Initialization
		EditorAssetManager::instantiate(SLD_BOWL_RESOURCE_OUTPUT_LOCATION, ".editor/asset_registry.xml");

		m_EditorUI.setViewportRenderId(m_PostProcessingComposer.getFramebuffer()->getColorAttachment(0));
		std::function<void()> func = std::bind(&EditorLayer::onViewportResized, this);
		//m_EditorUI.setViewportResizeHandler(func);
		//m_EditorUI.setEditorCamera(&m_EditorCamera);

		// Create a new instance of the editor selection context
		EditorSelectionContext::s_Instance = new EditorSelectionContext();

		EditorSettings::s_Instance->deserializeSettings();

		EditorEventManager::s_Instance = new EditorEventManager();
		EditorEventManager::registerListener(&m_EditorUI);
		EditorEventManager::registerListener(&m_EditorScene);
		EditorEventManager::registerListener(this);
	}

	void EditorLayer::onDetach() {
		delete EditorSelectionContext::s_Instance;
		EditorSelectionContext::s_Instance = nullptr;

		delete EditorSettings::s_Instance;
		EditorSettings::s_Instance = nullptr;

		delete EditorAssetManager::s_Instance;
		EditorAssetManager::s_Instance = nullptr;

		delete EditorEventManager::s_Instance;
		EditorEventManager::s_Instance = nullptr;
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

		m_EditorScene.setViewportHovered(m_EditorUI.isViewportHovered());
		m_EditorScene.setViewportFocused(m_EditorUI.isViewportFocused());

		m_EditorScene.onUpdate(ts);

		if(m_EditorScene.getEditorState() != EditorState::Runtime && m_EditorUI.isMouseInViewport()) {
			int pixeldata = m_Framebuffer->readPixel(1, m_EditorUI.getViewportMouseX(), m_EditorUI.getViewportMouseY());
			if(pixeldata != clearData) {
				m_HoveredEntity = { (uint32_t)pixeldata, &m_EditorScene };
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
		if(!e.handled) m_EditorScene.getEditorCamera().onEvent(e);
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
			case SLD_KEY_R: m_EditorUI.setGizmoType(ImGuizmo::OPERATION::SCALE); break;
			case SLD_KEY_ESCAPE: EditorSelectionContext::setSelectionContextNone(); break;

			// Runtime Hotkeys
			case SLD_KEY_F5: m_EditorScene.startRuntime(); break;
			case SLD_KEY_F6: m_EditorScene.pauseRuntime(); break;
			case SLD_KEY_F7: m_EditorScene.stopRuntime(); break;
		}
	}

	bool EditorLayer::canMousePick() {
		return m_EditorScene.getEditorState() != EditorState::Runtime && m_HoveredEntity.isValid() && !m_EditorUI.isGizmoHovered();
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

	void EditorLayer::onEditorEvent(const EditorEvent& evnt) {
		switch(evnt.type) {
			case EditorEventType::ViewportResized:
				EditorEventViewportResized& resizeEvent = (EditorEventViewportResized&)evnt;

				uint32_t width = resizeEvent.width;
				uint32_t height = resizeEvent.height;

				m_Framebuffer->resize(width, height);
				m_PostProcessingFramebufferResize = true;
				m_PostProcessingFramebufferWidth = width;
				m_PostProcessingFramebufferHeight = height;

				// TODO: Let camera handle viewport resizing based on projection type
				PerspectiveCameraProperties camProps;
				camProps.AspectRatio = (float)width / (float)height;
				m_EditorScene.getEditorCamera().setPerspectiveProjection(camProps);
				m_EditorScene.getEditorCamera().recalculateViewMatrix();

				Renderer::onWindowResized((uint32_t)width, (uint32_t)height);
				break;
		}
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
		m_EditorScene.getEditorCamera().setPerspectiveProjection(camProps);
		m_EditorScene.getEditorCamera().recalculateViewMatrix();

		Renderer::onWindowResized((uint32_t)width, (uint32_t)height);
	}

	void EditorLayer::onImGuiRender() { 
		m_EditorUI.setHoveredEntity(m_HoveredEntity);
		m_EditorUI.setCurrentFrames(m_CountedFrames);
		m_EditorUI.onImGuiRender();
	}

	void EditorLayer::serialize() {
		//SceneSerializer serializer(m_EditorScene);
		//serializer.serializeScene("assets/scenes/example.sld");
	}

	void EditorLayer::deserialize() {}

}
