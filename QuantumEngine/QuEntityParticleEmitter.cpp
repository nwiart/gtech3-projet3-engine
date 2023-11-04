#include "Quantum/Math/Math.h"
#include "QuEntityParticleEmitter.h"

using namespace DirectX;



QuEntityParticleEmitter::QuEntityParticleEmitter(float lifeTime, float spawnRate)
	: m_lifeTime(5.0F), m_spawnRate(1.0F)
	, m_spawnTimer(0.0F), m_spawnIndex(0)
{
	int maxParticles = this->getMaxParticles();

	m_particlePositions = reinterpret_cast<XMFLOAT4*>(malloc(maxParticles * sizeof(XMFLOAT4)));
	m_particleSizes = reinterpret_cast<XMFLOAT2*>(malloc(maxParticles * sizeof(XMFLOAT2)));
}

QuEntityParticleEmitter::~QuEntityParticleEmitter()
{
	if (m_particlePositions) {
		free(m_particlePositions);
		free(m_particleSizes);
	}
}

void QuEntityParticleEmitter::OnUpdate(Timer timer)
{
	m_spawnTimer += timer.getDeltaTime();

	while (m_spawnTimer >= m_spawnRate) {
		m_spawnTimer -= m_spawnRate;
		this->spawnParticle();
	}
}



int QuEntityParticleEmitter::getMaxParticles() const
{
	return ceil(m_lifeTime / m_spawnRate);
}


void QuEntityParticleEmitter::spawnParticle()
{
	XMVECTOR worldPos = this->getWorldPosition();



	// Increment spawn index and loop around.
	m_spawnIndex++;
	if (m_spawnIndex == this->getMaxParticles()) {
		m_spawnIndex = 0;
	}
}
