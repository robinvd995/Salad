#include "sldpch.h"
#include "OpenGLRenderAPI.h"

#include <glad/glad.h>

namespace Salad {

	void OpenGLRenderAPI::init() {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
	}

	void OpenGLRenderAPI::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		SLD_CORE_INFO("Viewport set: {0}, {1}, {2}, {3}", x, y, width, height);
		glViewport(x, y, width, height);
	}

	void OpenGLRenderAPI::setClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRenderAPI::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void OpenGLRenderAPI::drawIndexed(const Ref<VertexArray>& vertexArray) {
		glDrawElements(GL_TRIANGLES, vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRenderAPI::drawTriangles(const Ref<VertexArray>& vertexArray, uint32_t count) {
		glDrawArrays(GL_TRIANGLES, 0, count);
	}

	void OpenGLRenderAPI::depthMask(bool enabled) {
		glDepthMask(enabled ? GL_TRUE : GL_FALSE);
	}

}
