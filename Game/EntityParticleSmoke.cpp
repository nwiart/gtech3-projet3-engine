#include "EntityParticleSmoke.h"

#include <Quantum/Math/Math.h>



EntityParticleSmoke::EntityParticleSmoke(Texture2D* t)
	: QuEntityParticleEmitter(3.0F, 0.4F)
{
	this->setTexture(t);
}

void EntityParticleSmoke::UpdateParticles(const Timer& t)
{
	DirectX::XMFLOAT2* siz = this->getParticleSizes();
	DirectX::XMFLOAT4* col = this->getParticleColors();

	for ( ; siz != this->getParticleSizes() + this->getMaxParticles(); siz++, col++) {

		siz->x = siz->y += t.getDeltaTime();

		col->w -= t.getDeltaTime() * 0.33333F;
	}
}

void EntityParticleSmoke::OnParticleSpawn(int id)
{
	QuEntityParticleEmitter::OnParticleSpawn(id);

	this->getParticleRotations()[id] = Quantum::Math::randomFloat(-180.0F, 180.0F);
}
