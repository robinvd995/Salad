#include "sldpch.h"
#include "GuiRenderer.h"

#include "Salad/Renderer/Renderer2D.h"

namespace Salad {

	GuiElementRenderPackage* GuiRenderer::submitRenderPackageImpl(uint32_t x, uint32_t y, Ref<VertexArray> renderId, uint32_t layer) {
		GuiElementRenderPackage* pckg = new GuiElementRenderPackage(x, y, renderId, layer);
		m_RenderPackages.push_back(pckg);
		return pckg;
	}

	void GuiRenderer::renderImpl() {
		for(int i = 0; i < m_RenderPackages.size(); i++) {
			GuiElementRenderPackage* pckg = m_RenderPackages[i];
			Renderer2D::drawVertexArray({ pckg->getPosX(), pckg->getPosY(), 0 }, { 1.0f, 1.0f }, pckg->getRenderId());
		}
		
	}

}