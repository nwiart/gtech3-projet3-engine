#pragma once

#include "D3D12ConstantBuffer.h"

#include "ShaderUI.h"
#include "QuWidget.h"

#include"UIpass.h"

class UIRenderer
{
public:
	void init();
	void destroy();

	void render(ID3D12GraphicsCommandList* cmdList);

	void freeList();

	void visitUI(QuWidget* widget);


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
	};
	std::vector<RectanglesBuffer> renderRectangles;

	D3D12ConstantBuffer<RectanglesBuffer> m_cbRectangleData;

	CanvasConstantBuffer m_matrix;

	std::vector<QuWidget*> allWidget;

	ID3D12DescriptorHeap* m_Heap;

	void updateObjectCB();
};

