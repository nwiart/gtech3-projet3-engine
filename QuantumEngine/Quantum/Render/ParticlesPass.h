#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "D3D12ConstantBuffer.h"

#include "Quantum/Render/ShaderParticles.h"

#include <vector>

#define MAX_PARTICLES 256

class QuEntityParticleEmitter;

class D3D12Texture;



class ParticlesPass
{
public:

	void init();
	void destroy();

	void addParticleEmitterData(QuEntityParticleEmitter* em);

	void render(ID3D12GraphicsCommandList* cmdList);


private:

	struct Batch
	{
		D3D12Texture* m_texture;
		int m_numParticles;
	};

	std::vector<Batch> m_renderBatches;


	struct FrameData
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	D3D12ConstantBuffer<FrameData> m_cbFrameData;


	VertexBuffer m_quadVB;

	ID3D12Resource* m_particleBuffers[4];
	D3D12_VERTEX_BUFFER_VIEW m_particleBufferViews[4];

	ShaderParticles m_shader;
};
