#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include "Salad/Debug/Profiler.h"

//#include <lua53/include/lua.hpp>
//#include <LuaBridge/LuaBridge.h>

Sandbox2D::Sandbox2D() :
	Layer("Sandbox2D"),
	m_CameraController(1280.0f / 720.0f, true)
{
	/*{
		lua_State* luaState = luaL_newstate();
		luaL_dofile(luaState, "assets/scripts/script.lua");
		luaL_openlibs(luaState);
		lua_pcall(luaState, 0, 0, 0);

		luabridge::LuaRef str = luabridge::getGlobal(luaState, "testString");
		luabridge::LuaRef numb = luabridge::getGlobal(luaState, "number");
		std::string luaString = str.cast<std::string>();
		int luaNumber = numb.cast<int>();

		SLD_INFO("Lua String: {0}", luaString);
		SLD_INFO("Lua Number: {0}", luaNumber);
	}*/
}

void Sandbox2D::onAttach() {
	m_Texture = Salad::Texture2D::create("assets/textures/logo.png");
	Salad::Ref<Salad::Texture2D> spriteMap = Salad::Texture2D::create("assets/textures/spritemap.png");
	m_TextureMap = Salad::createRef<Salad::TextureMap>(spriteMap, 2, 2);
	m_Sprite = Salad::createRef<Salad::Sprite>(m_TextureMap, 4);
}

void Sandbox2D::onDetach() {
	
}

void Sandbox2D::onUpdate(Salad::Timestep ts) {
	SLD_PROFILER_FUNC();

	{
		// Update
		SLD_PROFILER_SCOPE("Update");
		m_CameraController.onUpdate(ts);
	}

	{
		SLD_PROFILER_SCOPE("Render");
		//Render
		Salad::RenderCommand::setClearColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
		Salad::RenderCommand::clear();

		Salad::Renderer2D::beginScene(m_CameraController.getCamera());
		Salad::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.2f, 1.0f });
		Salad::Renderer2D::drawTexturedQuad({ 1.0f, 0.0f }, glm::vec2(1.0f), m_Texture);
		Salad::Renderer2D::drawSprite({ 0.0f, 0.0f }, glm::vec2(1.0f), m_Sprite);
		Salad::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender() {
	ImGui::Begin("Settings");
	ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SqColor));
	ImGui::End();
}

void Sandbox2D::onEvent(Salad::Event& e) {
	m_CameraController.onEvent(e);
}
