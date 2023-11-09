#pragma once

#include "VertexBuffer.h"
#include "ShaderSun.h"

#include "D3D12ConstantBuffer.h"



class SunPass
{
public:

	void init();

	void render(ID3D12GraphicsCommandList* cmdList);


private:

	struct DataConstantBuffer
	{
		DirectX::XMFLOAT4X4 viewProjection;
		DirectX::XMFLOAT4 dirLightDirection;
	};


	VertexBuffer m_clipSpaceQuadVB;
	ShaderSun m_shader;

	D3D12ConstantBuffer<DataConstantBuffer> m_cbData;
};
