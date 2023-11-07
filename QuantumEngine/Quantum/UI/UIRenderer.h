#pragma once

#include "D3D12ConstantBuffer.h"

#include "ShaderUI.h"
#include "QuWidget.h"

#include"UIpass.h"

class QuWidgetText;

class UIRenderer
{
public:
	void init();
	void destroy();

	void render(ID3D12GraphicsCommandList* cmdList);

	void freeList();

	void addWidget(QuWidget* widget);

private:
	UIpass m_pass;

	struct CanvasConstantBuffer
	{
		DirectX::XMFLOAT4X4 matrix;
	};
	D3D12ConstantBuffer<CanvasConstantBuffer> m_cbCanvasData;

	struct RectanglesBuffer
	{
		DirectX::XMFLOAT2 position;
		DirectX::XMFLOAT2 size;
		char padding[256-16];
	};
	std::vector<RectanglesBuffer> renderRectangles;

	D3D12ConstantBuffer<RectanglesBuffer> m_cbRectangleData;

	CanvasConstantBuffer m_matrix;

	std::vector<QuWidget*> allWidget;

	std::vector<QuWidgetText*> allWidgetText;

	ID3D12DescriptorHeap* m_Heap;

	void updateObjectCB();
};

