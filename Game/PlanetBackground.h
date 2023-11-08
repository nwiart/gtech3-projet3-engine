#pragma once
#include "QuEntity.h"

class PlanetBackground : public QuEntity
{
	public:
	PlanetBackground();

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
};

