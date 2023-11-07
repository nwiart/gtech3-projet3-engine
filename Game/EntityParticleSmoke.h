#pragma once

#include <QuEntityParticleEmitter.h>

class Texture2D;



class EntityParticleSmoke : public QuEntityParticleEmitter
{
public:

	EntityParticleSmoke(Texture2D* t);

	virtual void UpdateParticles(const Timer& t) override;
	virtual void OnParticleSpawn(int id) override;
};
