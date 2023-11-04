#pragma once

#include "QuEntity.h"



class QuEntityParticleEmitter : public QuEntity
{
public:

	QuEntityParticleEmitter(float lifeTime, float spawnRate);
	virtual ~QuEntityParticleEmitter();

	virtual void OnUpdate(Timer timer) override;

	inline void setTexture(Texture2D* t) { m_texture = t; }

		/// Calculate the maximum number of particles that can exist at a time
		/// from this emitter.
	int getMaxParticles() const;


private:

	void spawnParticle();



private:

	DirectX::XMFLOAT4* m_particlePositions;
	DirectX::XMFLOAT2* m_particleSizes;

	float m_lifeTime;
	float m_spawnRate;

	float m_spawnTimer;
	int m_spawnIndex;

	Texture2D* m_texture;
};
