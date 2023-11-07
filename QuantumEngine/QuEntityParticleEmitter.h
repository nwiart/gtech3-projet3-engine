#pragma once

#include "QuEntity.h"

class Texture2D;



class QuEntityParticleEmitter : public QuEntity
{
public:

	QuEntityParticleEmitter(float lifeTime, float spawnRate);
	virtual ~QuEntityParticleEmitter();

	virtual void OnUpdate(const Timer& timer) final override;

	virtual void UpdateParticles(const Timer& timer);
	virtual void OnParticleSpawn(int id);

	inline const DirectX::XMFLOAT4* getParticlePositions() const { return m_particlePositions; }
	inline const float*             getParticleRotations() const { return m_particleRotations; }
	inline const DirectX::XMFLOAT2* getParticleSizes() const { return m_particleSizes; }
	inline const DirectX::XMFLOAT4* getParticleColors() const { return m_particleColors; }

	inline Texture2D* getTexture() const { return m_texture; }
	inline void setTexture(Texture2D* t) { m_texture = t; }

		/// Calculate the maximum number of particles that can exist at a time
		/// from this emitter.
	int getMaxParticles() const;

protected:

	inline DirectX::XMFLOAT4* getParticlePositions() { return m_particlePositions; }
	inline float*             getParticleRotations() { return m_particleRotations; }
	inline DirectX::XMFLOAT2* getParticleSizes() { return m_particleSizes; }
	inline DirectX::XMFLOAT4* getParticleColors() { return m_particleColors; }


private:

	void spawnParticle();

	virtual void ExecuteProcedure() override;



protected:

		/// Particle buffers.
	DirectX::XMFLOAT4* m_particlePositions;
	float*             m_particleRotations;
	DirectX::XMFLOAT2* m_particleSizes;
	DirectX::XMFLOAT4* m_particleColors;

private:

	float m_lifeTime;
	float m_spawnRate;

	float m_spawnTimer;
	int m_spawnIndex;

	Texture2D* m_texture;
};
