#include "UIRenderer.h"
#include "Graphics.h"

void UIRenderer::init()
{
	m_cbCanvasData.init(1);
	m_cbRectangleData.init(128);

	DirectX::XMMATRIX Matrix = XMMatrixOrthographicOffCenterLH(0,Graphics::getInstance().getRenderWidth(), Graphics::getInstance().getRenderHeight(),0,0,1);
	XMStoreFloat4x4(&m_matrix.matrix, Matrix);
	m_pass.init();
}

void UIRenderer::destroy()
{
	m_pass.destroy();

	m_cbCanvasData.destroy();
	m_cbRectangleData.destroy();
}

void UIRenderer::render(ID3D12GraphicsCommandList* cmdList)
{
	visitUI();

	Graphics& g = Graphics::getInstance();
	this->updateObjectCB();

	UINT cb_objectData_IDbase = g.allocateDescriptorTable(allWidget.size());
	for (int i = 0; i < allWidget.size(); i++)
	{
		g.setGlobalDescriptor(cb_objectData_IDbase + i, m_cbCanvasData.getDescriptor(i));
	}

	UINT cb_matrix_IDbase = g.allocateDescriptorTable(1);
	
	g.setGlobalDescriptor(cb_matrix_IDbase, m_cbCanvasData.getDescriptor());

	m_pass.render(cmdList, allWidget, cb_objectData_IDbase, cb_matrix_IDbase);
}

void UIRenderer::freeList()
{
	allWidget.clear();
}

void UIRenderer::visitUI(QuWidget* widget)
{
	if (widget == nullptr) return;

	allWidget.push_back(widget);

	RectanglesBuffer rect;
	rect.position = widget->GetPosition();
	rect.size = widget->GetSize();

	renderRectangles.push_back(rect);

	for (QuWidget* child = widget->m_FirstChild; child != nullptr; child = child->m_Sibling)
	{
		visitUI(widget);
	}
}

void UIRenderer::updateObjectCB()
{
	m_cbCanvasData.update(0, m_matrix);
	m_cbRectangleData.updateRange(0, renderRectangles.size(),renderRectangles.data());
}
