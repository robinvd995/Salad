#pragma once

#include "GuiBase.h"
#include "Salad/Renderer/VertexArray.h"

namespace Salad {

	class GuiRenderCommand {
	public:
		virtual void execute() = 0;
	};

	class GuiElementRenderPackage {

	public:
		//TODO: texture
		//TextureHandle texture;

		//TODO: scissor
		//bool scissorBounds = false;
		//uint32_t scissorWidth, uint32_t scissorHeight

		GuiElementRenderPackage() = delete;
		GuiElementRenderPackage(GuiElementRenderPackage&) = delete;
		GuiElementRenderPackage(uint32_t x, uint32_t y, Ref<VertexArray> rid, uint32_t renderLayer) :
			m_PosX(x), m_PosY(y), m_RenderId(rid), m_RenderLayer(renderLayer)
		{};

		inline uint32_t getPosX() { return m_PosX; }
		inline uint32_t getPosY() { return m_PosY; }
		inline Ref<VertexArray> getRenderId() { return m_RenderId; }

		void updatePosition(uint32_t x, uint32_t y) { m_PosX = x; m_PosY = y; }

	private:
		uint32_t m_PosX, m_PosY;
		Ref<VertexArray> m_RenderId;
		uint32_t m_RenderLayer;
	};

	class GuiRenderer {
	
	public:
		static void initializeRenderer() { if(!s_Instance) s_Instance = new GuiRenderer(); }

		static GuiElementRenderPackage* submitRenderPackage(uint32_t x, uint32_t y, Ref<VertexArray> renderId, uint32_t layer) { return s_Instance->submitRenderPackageImpl(x, y, renderId, layer); }
		static void render() { s_Instance->renderImpl(); }

	private:
		inline static GuiRenderer* s_Instance;

	public:
		GuiRenderer() {}
		~GuiRenderer() {}

		GuiElementRenderPackage* submitRenderPackageImpl(uint32_t x, uint32_t y, Ref<VertexArray> renderId, uint32_t layer);
		void renderImpl();

	private:
		std::vector<GuiElementRenderPackage*> m_RenderPackages;
	};

}