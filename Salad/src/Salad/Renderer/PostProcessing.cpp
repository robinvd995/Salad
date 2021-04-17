#include "sldpch.h"
#include "PostProcessing.h"

#include "RenderCommand.h"

namespace Salad {

	float s_FramebufferVertexData[] = {
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,

		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	PostProcessingEffect::PostProcessingEffect(Ref<Shader> shader, Ref<Framebuffer> framebuffer) :
		m_Shader(shader), m_Framebuffer(framebuffer)
	{
		m_VertexArray = VertexArray::create();

		Ref<Salad::VertexBuffer> vbo = VertexBuffer::create(s_FramebufferVertexData, sizeof(s_FramebufferVertexData), SLD_STATIC_DRAW);
		vbo->setLayout({
			{ "a_Position", Salad::ShaderDataType::Float2 },
			{ "a_TexCoord", Salad::ShaderDataType::Float2 }
		});

		m_VertexArray->addVertexBuffer(vbo);
		m_VertexArray->unbind();
	}

	void PostProcessingEffect::draw(Ref<Framebuffer> otherFramebuffer) {
		m_Framebuffer->bind();

		RenderCommand::clear();

		float clear[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_Framebuffer->clearColorBuffer(0, clear);

		m_Shader->bind();
		m_VertexArray->bind();

		m_Shader->setFloat4("u_OutlineColor", { 0.3f, 0.8f, 1.0f, 1.0f });

		otherFramebuffer->bindColorBufferAsTexture(2, 0);

		RenderCommand::drawTriangles(m_VertexArray, 6);
		m_Framebuffer->unbind();
	}

	PostProcessingComposer::PostProcessingComposer(Ref<Shader> shader, Ref<Framebuffer> framebuffer) :
		m_Shader(shader), m_Framebuffer(framebuffer) 	{
		m_VertexArray = VertexArray::create();

		Ref<Salad::VertexBuffer> vbo = VertexBuffer::create(s_FramebufferVertexData, sizeof(s_FramebufferVertexData), SLD_STATIC_DRAW);
		vbo->setLayout({
			{ "a_Position", Salad::ShaderDataType::Float2 },
			{ "a_TexCoord", Salad::ShaderDataType::Float2 }
			});

		m_VertexArray->addVertexBuffer(vbo);
		m_VertexArray->unbind();
	}

	void PostProcessingComposer::draw(Ref<Framebuffer> base, Ref<Framebuffer> overlay) {
		m_Framebuffer->bind();

		RenderCommand::clear();

		float clear[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		m_Framebuffer->clearColorBuffer(0, clear);

		m_Shader->bind();
		m_VertexArray->bind();

		m_Shader->setInt("u_BaseTexture", 0);
		m_Shader->setInt("u_OverlayTexture", 1);

		base->bindColorBufferAsTexture(0, 0);
		overlay->bindColorBufferAsTexture(0, 1);

		RenderCommand::drawTriangles(m_VertexArray, 6);
		m_Framebuffer->unbind();
	}

}