#include "EntityParticleSmoke.h"

#include <Quantum/Math/Math.h>



EntityParticleSmoke::EntityParticleSmoke(Texture2D* t)
	: QuEntityParticleEmitter(4.0F, 0.2F)
{
	this->setTexture(t);
}

void EntityParticleSmoke::UpdateParticles(const Timer& t)
{
	DirectX::XMFLOAT2* siz = this->getParticleSizes();
	DirectX::XMFLOAT4* col = this->getParticleColors();

	for (int i = 0; siz != this->getParticleSizes() + this->getMaxParticles(); siz++, col++, i++) {

		// Grow with time.
		siz->x = siz->y += t.getDeltaTime() * 1.4F;

		// Fade in and out.
		col->w = sin(this->getParticleLifetime(i) / 4.0F * DirectX::XM_PI);
	}
}

void EntityParticleSmoke::OnParticleSpawn(int id)
{
	QuEntityParticleEmitter::OnParticleSpawn(id);

	// Give particles random rotation.
	this->getParticleRotations()[id] = Quantum::Math::randomFloat(-180.0F, 180.0F);
}
