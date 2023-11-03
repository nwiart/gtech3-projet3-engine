#include "UIRenderer.h"
#include "Graphics.h"

void UIRenderer::init()
{
	m_cbCanvasData.init(1);

	DirectX::XMMATRIX Matrix = XMMatrixOrthographicOffCenterLH(0,Graphics::getInstance().getRenderWidth(), Graphics::getInstance().getRenderHeight(),0,0,1);
	XMStoreFloat4x4(&m_matrix.matrix, Matrix);
	m_pass.init();
}

void UIRenderer::destroy()
{
	m_pass.destroy();

	m_cbCanvasData.destroy();
}

void UIRenderer::render(ID3D12GraphicsCommandList* cmdList)
{
	this->updateObjectCB();

	m_pass.render(cmdList, allWidget);
}

void UIRenderer::visitUI(QuWidget* widget)
{
	if (widget == nullptr) return;

	allWidget.push_back(widget);


	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitUI(widget);
	}
}

void UIRenderer::updateObjectCB()
{
	m_cbCanvasData.update(0, m_matrix);
}
