#pragma once

#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Quantum/Render/ShaderParticles.h"

class QuEntityParticleEmitter;



class ParticlesPass
{
public:

	void init();
	void destroy();

	void addParticleEmitterData(QuEntityParticleEmitter* em);


private:

	VertexBuffer m_quadVB;

	ID3D12Resource* m_particleBuffers[4];
	D3D12_VERTEX_BUFFER_VIEW m_particleBufferViews[4];

	ShaderParticles m_shader;
};
