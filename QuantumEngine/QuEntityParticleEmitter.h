#pragma once

#include "QuEntity.h"

class Texture2D;



class QuEntityParticleEmitter : public QuEntity
{
public:

	QuEntityParticleEmitter(float lifeTime, float spawnRate);
	virtual ~QuEntityParticleEmitter();

	virtual void OnUpdate(Timer timer) final override;

	virtual void UpdateParticles(Timer timer);

	inline const DirectX::XMFLOAT4* getParticlePositions() const { return m_particlePositions; }
	inline const DirectX::XMFLOAT2* getParticleSizes() const { return m_particleSizes; }

	inline Texture2D* getTexture() const { return m_texture; }
	inline void setTexture(Texture2D* t) { m_texture = t; }

		/// Calculate the maximum number of particles that can exist at a time
		/// from this emitter.
	int getMaxParticles() const;


private:

	void spawnParticle();



protected:

		/// Particle buffers.
	DirectX::XMFLOAT4* m_particlePositions;
	DirectX::XMFLOAT2* m_particleSizes;

private:

	float m_lifeTime;
	float m_spawnRate;

	float m_spawnTimer;
	int m_spawnIndex;

	Texture2D* m_texture;
};
