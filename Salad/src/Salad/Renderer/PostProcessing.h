#pragma once

#include "Salad/Core/Core.h"
#include "Framebuffer.h"
#include "VertexArray.h"

#include "Shader.h"

namespace Salad {

	class PostProcessingEffect {

	public:
		PostProcessingEffect() {};
		PostProcessingEffect(Ref<Shader> shader, Ref<Framebuffer> framebuffer);
		~PostProcessingEffect() = default;

		void draw(Ref<Framebuffer> otherFramebuffer);

		Ref<Framebuffer> getFramebuffer() { return m_Framebuffer; }

	private:
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray> m_VertexArray;

		Ref<Framebuffer> m_OtherFramebuffer;
	};

	class PostProcessingComposer {

	public:
		PostProcessingComposer() {};
		PostProcessingComposer(Ref<Shader> shader, Ref<Framebuffer> framebuffer);
		~PostProcessingComposer() = default;

		void draw(Ref<Framebuffer> baseTexture, Ref<Framebuffer> overlayTexture);

		Ref<Framebuffer> getFramebuffer() { return m_Framebuffer; }

	private:
		Ref<Shader> m_Shader;
		Ref<Framebuffer> m_Framebuffer;
		Ref<VertexArray> m_VertexArray;

		Ref<Framebuffer> m_OtherFramebuffer;
	};

}