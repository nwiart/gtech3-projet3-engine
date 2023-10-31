#include "stdafx.h"
#include "SkyboxRenderer.h"

#include "D3D12Texture.h"

#include "Graphics.h"



static struct { float pos[3]; } verts[] = {
	{ {-1.0F, -1.0F, -1.0F} },
	{ {-1.0F, +1.0F, -1.0F} },
	{ {+1.0F, +1.0F, -1.0F} },
	{ {+1.0F, -1.0F, -1.0F} },
	{ {-1.0F, -1.0F, +1.0F} },
	{ {-1.0F, +1.0F, +1.0F} },
	{ {+1.0F, +1.0F, +1.0F} },
	{ {+1.0F, -1.0F, +1.0F} },
};

static UINT indices[] = {
	// front face
	0, 1, 2,
	0, 2, 3,
	// back face
	4, 6, 5,
	4, 7, 6,
	// left face
	4, 5, 1,
	4, 1, 0,
	// right face
	3, 2, 6,
	3, 6, 7,
	// top face
	1, 5, 6,
	1, 6, 2,
	// bottom face
	4, 0, 3,
	4, 3, 7
};



void SkyboxRenderer::init()
{
	m_textureCube = 0;

	m_boxVB.setData(verts, sizeof(verts), 3 * sizeof(float));
	m_boxIB.setData(indices, sizeof(indices));

	m_shader.init();

	m_skyboxTexture.loadFromDisk("textures/milkyway.dds", D3D12_SRV_DIMENSION_TEXTURECUBE);
	m_textureCube = &m_skyboxTexture;
}

void SkyboxRenderer::destroy()
{
	m_skyboxTexture.destroy();

	m_boxVB.destroy();
	m_boxIB.destroy();
}

void SkyboxRenderer::setTexture(D3D12Texture* tex)
{
	m_textureCube = tex;
}

void SkyboxRenderer::render(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();

	UINT skyboxTextureID = g.allocateDescriptorTable(1);
	g.setGlobalDescriptor(skyboxTextureID, m_textureCube->getShaderResourceView());

	// Bind shader data.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(Graphics::getInstance().getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), 0, Graphics::getInstance().getCBVDescriptorSize()));
	cmdList->SetGraphicsRootDescriptorTable(
		0,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(Graphics::getInstance().getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), skyboxTextureID, Graphics::getInstance().getCBVDescriptorSize()));

	// Bind box model.
	cmdList->IASetVertexBuffers(0, 1, &m_boxVB.getVertexBufferView());
	cmdList->IASetIndexBuffer(&m_boxIB.getIndexBufferView());
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw.
	cmdList->DrawIndexedInstanced(6 * 6, 1, 0, 0, 0);
}
