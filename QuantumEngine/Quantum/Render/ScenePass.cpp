#include "stdafx.h"
#include "ScenePass.h"

#include "Graphics.h"

#include "Model.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"



void ScenePass::init()
{
	m_shader.init();
	assert(m_shader.isReady());

	m_texture.loadFromDisk("textures/awesome_sphere.dds", D3D12_SRV_DIMENSION_TEXTURE2D);
}

void ScenePass::destroy()
{
	m_shader.destroy();

	m_texture.destroy();
}

void ScenePass::renderAll(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderModel>& renderList, UINT frameData, UINT matData, UINT objectBase)
{
	Graphics& g = Graphics::getInstance();

	// Bind shader.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	// Set tables.
	m_shader.setConstantBuffer(2, frameData);

	// Render object.
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (int i = 0; i < renderList.size(); i++)
	{
		cmdList->IASetVertexBuffers(0, 1, &renderList[i].model->GetVertexBuffer()->getVertexBufferView());
		cmdList->IASetIndexBuffer(&renderList[i].model->GetIndexBuffer()->getIndexBufferView());

		m_shader.setTexture2D(0, matData + i * 3);
		m_shader.setConstantBuffer(0, objectBase + i);

		cmdList->DrawIndexedInstanced(renderList[i].model->GetNumberTriangle() * 3, 1, 0, 0, 0);
	}
}
