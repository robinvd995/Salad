#pragma once

#include "RenderAPI.h"

namespace Salad {

	class RenderCommand {

	public:
		static inline void init() { s_RenderAPI->init(); };
		static inline void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { s_RenderAPI->setViewport(x, y, width, height); }
		static inline void setClearColor(const glm::vec4 color) { s_RenderAPI->setClearColor(color); }
		static inline void clear() { s_RenderAPI->clear(); }
		static inline void drawIndexed(const Ref<VertexArray>& vertexArray) { s_RenderAPI->drawIndexed(vertexArray); }

	private:
		static RenderAPI* s_RenderAPI;
	};
}