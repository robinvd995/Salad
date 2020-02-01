#include "sldpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Salad {

//#ifdef RENDER_API_OPENGL
	RenderAPI* RenderCommand::s_RenderAPI = new OpenGLRenderAPI();
//#endif
}