#pragma once

#include "Salad/Renderer/RenderAPI.h"

namespace Salad {

	class OpenGLRenderAPI : public RenderAPI {

		virtual void init() override;
		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void setClearColor(const glm::vec4& color) override;
		virtual void clear() override;

		virtual void drawIndexed(const Ref<VertexArray>& vertexArray) override;

		virtual void drawTriangles(const Ref<VertexArray>& vertexArray, uint32_t count) override;

		virtual void depthMask(bool enabled) override;

		virtual void enableScissorTest(bool enable) override;
		virtual void scissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

	};
}
