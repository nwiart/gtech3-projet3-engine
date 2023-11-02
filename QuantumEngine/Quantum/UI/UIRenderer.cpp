#include "UIRenderer.h"
#include "Graphics.h"

void UIRenderer::init()
{
	m_cbObjectData.init(1);
	matrix = XMMatrixOrthographicOffCenterLH(0,Graphics::getInstance().getRenderWidth(), Graphics::getInstance().getRenderHeight(),0,0,1);
	m_pass.init();
}

void UIRenderer::destroy()
{
	m_pass.destroy();

	m_cbObjectData.destroy();
}

void UIRenderer::render(ID3D12GraphicsCommandList* cmdList)
{
	m_pass.render(cmdList);
}

void UIRenderer::visitUI(QuWidget* widget)
{
	if (widget == nullptr) return;

	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitUI(widget);
	}
}

void UIRenderer::updateObjectCB()
{
	m_cbObjectData.updateRange(0, matrix, matrix);
}
