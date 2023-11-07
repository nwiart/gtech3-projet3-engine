#include "Quantum/Math/Math.h"
#include "QuEntityParticleEmitter.h"

#include <stdafx.h>
#include "Graphics.h"

using namespace DirectX;



QuEntityParticleEmitter::QuEntityParticleEmitter(float lifeTime, float spawnRate)
	: m_lifeTime(lifeTime), m_spawnRate(spawnRate)
	, m_spawnTimer(0.0F), m_spawnIndex(0)
{
	int maxParticles = this->getMaxParticles();

	m_particlePositions = reinterpret_cast<XMFLOAT4*>(malloc(maxParticles * sizeof(XMFLOAT4)));
	m_particleRotations = reinterpret_cast<float*>(malloc(maxParticles * sizeof(float)));
	m_particleSizes     = reinterpret_cast<XMFLOAT2*>(malloc(maxParticles * sizeof(XMFLOAT2)));
	m_particleColors    = reinterpret_cast<XMFLOAT4*>(malloc(maxParticles * sizeof(XMFLOAT4)));
}

QuEntityParticleEmitter::~QuEntityParticleEmitter()
{
	if (m_particlePositions) {
		free(m_particlePositions);
		free(m_particleRotations);
		free(m_particleSizes);
		free(m_particleColors);
	}
}

void QuEntityParticleEmitter::OnUpdate(const Timer& timer)
{
	m_spawnTimer += timer.getDeltaTime();

	while (m_spawnTimer >= m_spawnRate) {
		m_spawnTimer -= m_spawnRate;
		this->spawnParticle();
	}

	this->UpdateParticles(timer);
}

void QuEntityParticleEmitter::UpdateParticles(const Timer& timer)
{
	// Default movement.
	for (XMFLOAT4* pos = m_particlePositions; pos != m_particlePositions + this->getMaxParticles(); pos++) {
		XMVECTOR v = XMLoadFloat4(pos);
		v = XMVectorAdd(v, XMVectorReplicate(timer.getDeltaTime()));
		XMStoreFloat4(pos, v);
	}
}

void QuEntityParticleEmitter::OnParticleSpawn(int id)
{
	XMVECTOR worldPos = this->getWorldPosition();

	XMStoreFloat4(&m_particlePositions[id], worldPos);
	m_particleRotations[id] = 0.0F;
	XMStoreFloat2(&m_particleSizes[id], XMVectorSet(0.5F, 0.5F, 0, 0));
	XMStoreFloat4(&m_particleColors[id], XMVectorSplatOne());
}



int QuEntityParticleEmitter::getMaxParticles() const
{
	return ceil(m_lifeTime / m_spawnRate);
}


void QuEntityParticleEmitter::spawnParticle()
{
	this->OnParticleSpawn(m_spawnIndex);

	// Increment spawn index and loop around.
	m_spawnIndex++;
	if (m_spawnIndex == this->getMaxParticles()) {
		m_spawnIndex = 0;
	}
}

void QuEntityParticleEmitter::ExecuteProcedure()
{
	Graphics::getInstance().addParticleEmitter(this);
}
