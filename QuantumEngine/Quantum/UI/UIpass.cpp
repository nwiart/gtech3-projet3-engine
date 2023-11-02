#include "stdafx.h"
#include "UIpass.h"

#include "D3D12Texture.h"

#include "Graphics.h"

static struct { float pos[3]; } verts[] = {
	{ {+1.0F, 0.F, 0.F} },
	{ {0.F, 0.F, 0.F} },
	{ {0.F, +1.0F, 0.0F} },
	{ {+1.0F, +1.0F, 0.0F} },
	{ {0.F, +1.0F, 0.0F} },
	{ {+1.0F, 0.F, 0.0F} },
};

void UIpass::init()
{
	m_texture = 0;

	m_boxVB.setData(verts, sizeof(verts), 3 * sizeof(float));

	m_shader.init();
}

void UIpass::destroy()
{
	m_boxVB.destroy();
}

void UIpass::setTexture(D3D12Texture* tex)
{
}

void UIpass::render(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();

	// No texture bound.
	//if (!m_textureCube) return;

	//UINT skyboxTextureID = g.allocateDescriptorTable(1);
	//g.setGlobalDescriptor(skyboxTextureID, m_textureCube->getShaderResourceView());

	// Bind shader data.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	//cmdList->SetGraphicsRootDescriptorTable(
	//	0,
	//	CD3DX12_GPU_DESCRIPTOR_HANDLE(Graphics::getInstance().getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), skyboxTextureID, Graphics::getInstance().getCBVDescriptorSize()));

	// Bind box model.
	cmdList->IASetVertexBuffers(0, 1, &m_boxVB.getVertexBufferView());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw.
	cmdList->DrawInstanced(6,1,0,0);
}
