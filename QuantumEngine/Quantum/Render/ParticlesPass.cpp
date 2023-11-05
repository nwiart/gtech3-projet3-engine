#include "stdafx.h"
#include "ParticlesPass.h"

#include "Graphics.h"

using namespace DirectX;



static struct { float pos[3]; float uv[2]; } verts[] = {
	{ {-0.5F, -0.5F, 0.0F} },
	{ {-0.5F, +0.5F, 0.0F} },
	{ {+0.5F, +0.5F, 0.0F} },

	{ {+0.5F, +0.5F, 0.0F} },
	{ {+0.5F, -0.5F, 0.0F} },
	{ {-0.5F, -0.5F, 0.0F} },
};

struct ParticleInstanceData
{
	XMFLOAT4 position;
	XMFLOAT2 size;
};



void ParticlesPass::init()
{
	Graphics& g = Graphics::getInstance();

	m_quadVB.setData(verts, sizeof(verts), 5 * sizeof(float));

	m_shader.init();


	for (int i = 0; i < 4; ++i) {
		g.getDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(64 * sizeof(ParticleInstanceData)),
			D3D12_RESOURCE_STATE_COMMON,
			0,
			IID_PPV_ARGS(&m_particleBuffers[i]));

		m_particleBufferViews[i] = { m_particleBuffers[i]->GetGPUVirtualAddress(), 64 * sizeof(ParticleInstanceData), sizeof(ParticleInstanceData) };
	}
}

void ParticlesPass::destroy()
{
	m_shader.destroy();

	m_quadVB.destroy();
}

void ParticlesPass::addParticleEmitterData(QuEntityParticleEmitter* em)
{
	
}
