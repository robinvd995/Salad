#include "sldpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"
#include "Platform/OpenGL/OpenGLRenderFlags.h"

namespace Salad {

	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();
	RenderFlags* RenderCommand::s_RenderFlags = new OpenGLRenderFlags();
}