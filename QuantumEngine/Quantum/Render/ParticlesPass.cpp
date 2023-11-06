#include "stdafx.h"
#include "ParticlesPass.h"

#include "Graphics.h"
#include "Quantum/Render/RenderScene.h"

#include "QuEntityParticleEmitter.h"
#include "Texture2D.h"

using namespace DirectX;



static struct { float pos[3]; float uv[2]; } verts[] = {
	{ {-0.5F, -0.5F, 0.0F}, {0, 1} },
	{ {-0.5F, +0.5F, 0.0F}, {0, 0} },
	{ {+0.5F, +0.5F, 0.0F}, {1, 0} },

	{ {+0.5F, +0.5F, 0.0F}, {1, 0} },
	{ {+0.5F, -0.5F, 0.0F}, {1, 1} },
	{ {-0.5F, -0.5F, 0.0F}, {0, 1} },
};

struct ParticleInstanceData
{
	XMFLOAT3 position;
	XMFLOAT2 size;
};



void ParticlesPass::init()
{
	Graphics& g = Graphics::getInstance();

	m_quadVB.setData(verts, sizeof(verts), 5 * sizeof(float));
	m_cbFrameData.init(1);

	m_shader.init();


	for (int i = 0; i < 1; ++i) {
		g.getDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(MAX_PARTICLES * sizeof(ParticleInstanceData)),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			0,
			IID_PPV_ARGS(&m_particleBuffers[i]));

		m_particleBufferViews[i] = { m_particleBuffers[i]->GetGPUVirtualAddress(), MAX_PARTICLES * sizeof(ParticleInstanceData), sizeof(ParticleInstanceData) };
	}
}

void ParticlesPass::destroy()
{
	m_shader.destroy();

	m_quadVB.destroy();
}

void ParticlesPass::addParticleEmitterData(QuEntityParticleEmitter* em)
{
	ParticleInstanceData* data;
	m_particleBuffers[0]->Map(0, 0, (void**) &data);

	for (int i = 0; i < em->getMaxParticles(); ++i) {
		XMStoreFloat3(&data[i].position, XMLoadFloat4(&em->getParticlePositions()[i]));
		XMStoreFloat2(&data[i].size, XMLoadFloat2(&em->getParticleSizes()[i]));
	}

	m_particleBuffers[0]->Unmap(0, 0);


	Batch batch;
	batch.m_numParticles = em->getMaxParticles();
	batch.m_texture = em->getTexture() ? em->getTexture()->getTexture() : 0;

	m_renderBatches.push_back(batch);
}

void ParticlesPass::render(ID3D12GraphicsCommandList* cmdList)
{
	Graphics& g = Graphics::getInstance();

	// Update frame data.
	FrameData cb;
	{
		XMStoreFloat4x4(&cb.view, g.getRenderScene().getViewMatrix());
		XMStoreFloat4x4(&cb.projection, g.getRenderScene().getProjectionMatrix());

		m_cbFrameData.update(0, cb);
	}

	// Set shader.
	cmdList->SetPipelineState(m_shader.getPipelineStateObject());
	cmdList->SetGraphicsRootSignature(m_shader.getRootSignature());

	UINT frameDataID = g.allocateDescriptorTable(1);
	UINT textureID = g.allocateDescriptorTable(1);

	g.setGlobalDescriptor(frameDataID, m_cbFrameData.getDescriptor());
	cmdList->SetGraphicsRootDescriptorTable(
		2,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(g.getShaderVisibleCBVHeap()->GetGPUDescriptorHandleForHeapStart(), frameDataID, g.getCBVDescriptorSize()));

	//g.setGlobalDescriptor(matData + 1, m_texture.getShaderResourceView());
	//m_shader.setTexture2D(0, matData);

	// Set quad geometry and particle data.
	D3D12_VERTEX_BUFFER_VIEW vbViews[2];
	vbViews[0] = m_quadVB.getVertexBufferView();
	vbViews[1] = m_particleBufferViews[0];

	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	cmdList->IASetVertexBuffers(0, 2, vbViews);

	// Render particle batches.
	int numProcessedParticles = 0;
	for (const Batch& b : m_renderBatches)
	{
		cmdList->DrawInstanced(6, b.m_numParticles, 0, numProcessedParticles);

		numProcessedParticles += b.m_numParticles;
	}

	m_renderBatches.clear();
}
