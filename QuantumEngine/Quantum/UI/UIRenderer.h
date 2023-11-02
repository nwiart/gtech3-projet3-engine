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

	void visitUI(QuWidget* widget);


private:
	UIpass m_pass;

	struct CanvasConstantBuffer
	{
		DirectX::XMFLOAT4X4 matrix;
	};

	D3D12ConstantBuffer<CanvasConstantBuffer> m_cbObjectData;

	XMMATRIX matrix;

	void updateObjectCB();
};

