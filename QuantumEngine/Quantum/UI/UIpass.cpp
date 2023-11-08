#include "stdafx.h"
#include "UIpass.h"

#include "D3D12Texture.h"

#include "UIRenderer.h"

#include "QuWidgetText.h"

#include "Graphics.h"

static struct { float pos[3]; float uv[2]; } verts[] = {
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

void UIpass::renderRectangles(ID3D12GraphicsCommandList* cmdList, const std::vector<QuWidget*>& list, UINT ObjectBase, UINT matrix)
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

	m_shader.setConstantBuffer(1, matrix);
	// Draw.
	for (int i = 0; i < list.size(); i++)
	{

		m_shader.setConstantBuffer(0, ObjectBase + i);

		cmdList->DrawInstanced(6, 1, 0, 0);
	}
}

void UIpass::renderText(ID3D12GraphicsCommandList* cmdList, const std::vector<QuWidgetText*>& list, UINT ObjectBase, UINT matrix, UINT texture)
{
	Graphics& g = Graphics::getInstance();

	// No texture bound.
	//if (!m_textureCube) return;

	// Bind shader data.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	// Set texture atlas.
	cmdList->SetGraphicsRootDescriptorTable(
		1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(Graphics::getInstance().getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), 
			texture, Graphics::getInstance().getCBVDescriptorSize()));

	m_shader.setConstantBuffer(1, matrix);

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Draw.
	for (int i = 0; i < list.size(); i++)
	{
		// Bind model.
		cmdList->IASetVertexBuffers(0, 1, &list[i]->GetVertexBuffer()->getVertexBufferView());

		m_shader.setConstantBuffer(0, ObjectBase + i);

		cmdList->DrawInstanced(list[i]->GetNumOfVertices(), 1, 0, 0);
	}
}
