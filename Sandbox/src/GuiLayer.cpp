#include "sldpch.h"
#include "GuiLayer.h"

#include <Salad/Events/Event.h>

GuiLayer::GuiLayer() :
	m_Camera(0, 1280, 720, 0)
{
	m_Framebuffer = Salad::Framebuffer::create(1280, 720);
	m_GuiWindow = Salad::createRef<Salad::GuiWindow>(10, 10, 0.0f, 0.0f, 296, 160);
	Salad::Ref<Salad::WindowStyle> windowStyle = Salad::GuiWindow::loadWindowStyle("assets/misc/window_styles/default.lua");
	m_GuiWindow->setWindowStyle(windowStyle);
	m_GuiWindow->initWindow();

	//FONT STUFF
	m_Font = Salad::FontManager::loadFont("assets/fonts/arial.ttf", 48);
	m_Text = m_Font->generateText("This is a font test using the TrueType Library!");
}

void GuiLayer::onAttach() {

}

void GuiLayer::onDetach() {

}

void GuiLayer::onUpdate(Salad::Timestep ts) {
	Salad::Renderer2D::beginScene(m_Camera);
	
	//m_Framebuffer->bind();
	//Salad::RenderCommand::setClearColor({ 0.0f, 0.0f, 0.0f, 0.0f });
	//Salad::RenderCommand::clear();
	m_GuiWindow->drawWindow();
	m_Font->getFontTexture()->bind();
	Salad::Renderer2D::drawVertexArray({ 20.0f, 400.0f }, { 1.0f, 1.0f }, m_Text);
	//m_Framebuffer->unbind();
	Salad::Renderer2D::endScene();
}

void GuiLayer::onEvent(Salad::Event& e) {
	Salad::EventDispatcher dispatcher(e);
	dispatcher.dispatch<Salad::WindowResizeEvent>(SLD_BIND_EVENT_FN(GuiLayer::onWindowResizedEvent));
	m_GuiWindow->onEvent(e);
}

bool GuiLayer::onWindowResizedEvent(Salad::WindowResizeEvent& e) {
	m_Camera.setProjection(0, e.getWidth(), e.getHeight(), 0);
	return false;
}
