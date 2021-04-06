#pragma once

#include "Salad/Core/Core.h"

#include "Salad/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Salad {

	class OpenGLContext : public GraphicsContext {

	public:
		OpenGLContext(GLFWwindow* window);
		~OpenGLContext();

		virtual void init() override;
		virtual void swapBuffers() override;

	private:
		GLFWwindow* m_WindowHandle;
	};
}