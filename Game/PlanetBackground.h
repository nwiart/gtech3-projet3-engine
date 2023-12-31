#pragma once
#include "QuEntity.h"
#include <vector>
#include "EntityPlanetarySystem.h"
#include "ResourceLibrary.h"

class PlanetBackground : public QuEntity
{
	public:
	PlanetBackground();

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;

private:
	std::vector <Texture2D*> texture;
	std::vector<EntityPlanetarySystem*> m_planets;
	float X = 0;
	float Y = 0;
	float Z = 0;
};

