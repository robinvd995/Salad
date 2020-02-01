#include "sldpch.h"

#include "OpenGLContext.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Salad
{
	OpenGLContext::OpenGLContext(GLFWwindow* window) :
		m_WindowHandle(window)
	{
		SLD_CORE_ASSERT(window, "m_WindowHandle is null!");
	}

	OpenGLContext::~OpenGLContext() {}

	void OpenGLContext::init() {
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SLD_CORE_ASSERT(status, "Failed to intialize Glad!");

		SLD_CORE_INFO("OpenGL:");
		SLD_CORE_INFO("    Vendor:    {0}", glGetString(GL_VENDOR));
		SLD_CORE_INFO("    Renderer:  {0}", glGetString(GL_RENDERER));
		SLD_CORE_INFO("    Version:   {0}", glGetString(GL_VERSION));
	}

	void OpenGLContext::swapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}
}