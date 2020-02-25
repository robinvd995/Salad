#include "Sandbox2D.h"

#include <Platform/OpenGL/OpenGLShader.h>

#include <glm/glm/ext/matrix_transform.hpp>
#include <glm/glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>

#include "Salad/Debug/Profiler.h"

#include <Salad/Entity/Entity.h>
#include <Salad/Renderer/Sprite.h>
#include "Salad/Renderer/TextureManager.h"

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

	//SLD_TRACE("Component ID: {0}", component->getComponentType());

	/*Salad::SpriteAnimator animator(0.2f, 5);
	animator.updateAnimator({0.1f});
	SLD_TRACE("Animator: {0}, {1}, {2}, {3}", animator.getFrameTime(), animator.getFrameCount(), animator.getCurFrameTime(), animator.getCurFrame());
	
	Salad::Ref<Salad::EntityComponentTest> component = m_Entity->getComponent<Salad::EntityComponentTest>();
	if (component == nullptr) {
		SLD_TRACE("NULLPTR!");
	}
	else {
		SLD_TRACE("NOT NULLPTR!");
	}
	*/
}

void Sandbox2D::onAttach() {
	// Texture loading
	{
		//m_Texture = Salad::TextureManager::get().loadTexture2D("assets/textures/logo.png");
		Salad::Ref<Salad::Texture2D> t0 = Salad::TextureManager::get().loadTexture2D("assets/textures/spritemap_link.png");
		Salad::Ref<Salad::Texture2D> t1 = Salad::TextureManager::get().loadTexture2D("assets/textures/tileset.png");
	}


	// Sprite loading
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_move_down.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_move_up.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_move_left.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_move_right.sprite"));

	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_stand_down.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_stand_up.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_stand_left.sprite"));
	Salad::SpriteLibrary::get().loadSprite(std::string("assets/sprites/player_stand_right.sprite"));

	m_Entity = Salad::createRef<Salad::Entity>();
	Salad::Entity::attachComponent(m_Entity, Salad::createRef<Salad::EntityComponentTransform>());

	Salad::Ref<Salad::EntityComponentSpriteRenderer> spriteRenderer = Salad::createRef<Salad::EntityComponentSpriteRenderer>();
	spriteRenderer->setSprite("player_move_up");
	Salad::Entity::attachComponent(m_Entity, spriteRenderer);

	Salad::Entity::attachComponent(m_Entity, Salad::createRef<Salad::EntityComponentPlayerController>());
	m_CameraController.setTarget(m_Entity);

	m_TileMap = Salad::createRef<Salad::TileMap>(64, 64, 4, 1.0f, 1.0f);
	m_TileMap->buildTileMap();
}

void Sandbox2D::onDetach() {
}

void Sandbox2D::onUpdate(Salad::Timestep ts) {
	SLD_PROFILER_FUNC();

	{
		// Update
		SLD_PROFILER_SCOPE("Update");
		m_Entity->onUpdate(ts);
		m_CameraController.onUpdate(ts);
	}

	{
		// Render
		SLD_PROFILER_SCOPE("Render");
		Salad::RenderCommand::setClearColor(glm::vec4(1.0f, 0.05f, 0.05f, 1.0f));
		Salad::RenderCommand::clear();

		Salad::Renderer2D::beginScene(m_CameraController.getCamera());

		Salad::Ref<Salad::EntityComponentSpriteRenderer> ecsr = m_Entity->getComponent<Salad::EntityComponentSpriteRenderer>();
		Salad::Ref<Salad::EntityComponentTransform> transform = m_Entity->getComponent<Salad::EntityComponentTransform>();

		//Salad::Renderer2D::drawQuad({ -1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.2f, 1.0f });
		//Salad::Renderer2D::drawTexturedQuad({ 1.0f, 0.0f }, glm::vec2(1.0f), m_Texture);

		Salad::TextureManager::get().getTexture2D("assets/textures/tileset.png")->bind();
		Salad::Renderer2D::drawTileMap({ 0.0f, 0.0f }, glm::vec2(0.5f), m_TileMap);
		
		Salad::Renderer2D::drawSprite(transform->cpyPosition(), transform->cpyScale(), ecsr->getSpriteRenderInformation());

		Salad::Renderer2D::endScene();
	}
}

void Sandbox2D::onImGuiRender() {
	//ImGui::Begin("Settings");
	//ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SqColor));
	//ImGui::End();
}

void Sandbox2D::onEvent(Salad::Event& e) {
	m_CameraController.onEvent(e);
}
