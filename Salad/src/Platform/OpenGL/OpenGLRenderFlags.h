#pragma once

#include "Salad/Renderer/RenderFlags.h"
#include "glad/glad.h"

namespace Salad {

	class OpenGLRenderFlags : public RenderFlags {

	public:
		virtual unsigned int getFlagStaticDraw() const override { return GL_STATIC_DRAW; }
		virtual unsigned int getFlagDynamicDraw() const override { return GL_DYNAMIC_DRAW; }
	};
}