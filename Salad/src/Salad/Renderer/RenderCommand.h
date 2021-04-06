#pragma once

#include "RenderAPI.h"
#include "RenderFlags.h"

//Render Flag Defines
#define SLD_STATIC_DRAW RenderCommand::getFlags()->getFlagStaticDraw()
#define SLD_DYNAMIC_DRAW RenderCommand::getFlags()->getFlagDynamicDraw()

namespace Salad {

	class RenderCommand {

	public:
		static inline void init() { s_RenderAPI->init(); };
		static inline void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderAPI->setViewport(x, y, width, height); }
		static inline void setClearColor(const glm::vec4 color) { s_RenderAPI->setClearColor(color); }
		static inline void clear() { s_RenderAPI->clear(); }
		static inline void drawIndexed(const Ref<VertexArray>& vertexArray) { s_RenderAPI->drawIndexed(vertexArray); }
		static inline void drawTriangles(const Ref<VertexArray>& vertexArray, uint32_t count) { s_RenderAPI->drawTriangles(vertexArray, count); }
		static inline void depthMask(bool enable) { s_RenderAPI->depthMask(enable); }

		static inline RenderFlags* getFlags() { return s_RenderFlags; }

	private:
		static RenderAPI* s_RenderAPI;
		static RenderFlags* s_RenderFlags;
	};
}