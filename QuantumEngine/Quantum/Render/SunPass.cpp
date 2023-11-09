#include "stdafx.h"
#include "SunPass.h"

#include "Graphics.h"



static struct { float pos[2]; } verts[] =
{
	{ -1.0F, -1.0F },
	{ -1.0F,  1.0F },
	{  1.0F,  1.0F },
	{  1.0F,  1.0F },
	{  1.0F, -1.0F },
	{ -1.0F, -1.0F },
};



void SunPass::init()
{
	m_clipSpaceQuadVB.setData(verts, sizeof(verts), 2 * sizeof(float));

	m_cbData.init(1);
	m_shader.init();
}

void SunPass::render(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();
	
	DataConstantBuffer cb;
	{
		XMStoreFloat4x4(&cb.viewProjection, XMMatrixMultiply(g.getRenderScene().getProjectionMatrix(), g.getRenderScene().getViewMatrix()));
		XMStoreFloat4(&cb.dirLightDirection, XMVectorSet(0.0F, 0.0F, 1.0F, 0.0F));
		m_cbData.update(0, cb);
	}

	UINT dataID = g.allocateDescriptorTable(1);
	g.setGlobalDescriptor(dataID, m_cbData.getDescriptor());

	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	cmdList->SetGraphicsRootDescriptorTable(
		2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(g.getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), dataID, g.getCBVDescriptorSize()));

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 1, &m_clipSpaceQuadVB.getVertexBufferView());

	cmdList->DrawInstanced(6, 1, 0, 0);
}
