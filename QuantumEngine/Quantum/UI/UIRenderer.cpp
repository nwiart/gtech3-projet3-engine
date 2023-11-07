#include "UIRenderer.h"
#include "Graphics.h"
#include "QuWidgetText.h"

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

	Graphics& g = Graphics::getInstance();
	this->updateObjectCB();

	UINT cb_objectData_IDbase = g.allocateDescriptorTable(allWidget.size());
	for (int i = 0; i < allWidget.size(); i++)
	{
		g.setGlobalDescriptor(cb_objectData_IDbase + i, m_cbRectangleData.getDescriptor(i));
	}

	UINT cb_matrix_IDbase = g.allocateDescriptorTable(1);
	
	g.setGlobalDescriptor(cb_matrix_IDbase, m_cbCanvasData.getDescriptor());

	m_pass.render(cmdList, allWidget, cb_objectData_IDbase, cb_matrix_IDbase);
	freeList();
}

void UIRenderer::freeList()
{
	allWidget.clear();
	allWidgetText.clear();
	renderRectangles.clear();
}

void UIRenderer::addWidget(QuWidget* widget)
{
	RectanglesBuffer rect;
	rect.position = widget->GetPosition();
	rect.size = widget->GetSize();

	if (dynamic_cast<QuWidgetText*>(widget))
	{
		allWidgetText.push_back(dynamic_cast<QuWidgetText*>(widget));
	}
	else
	{
		renderRectangles.push_back(rect);
		allWidget.push_back(widget);
	}

}

void UIRenderer::updateObjectCB()
{
	m_cbCanvasData.update(0, m_matrix);
	m_cbRectangleData.updateRange(0, renderRectangles.size(),renderRectangles.data());
}
