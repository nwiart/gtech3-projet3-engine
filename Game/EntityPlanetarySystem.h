#pragma once

#include <QuEntity.h>
#include <vector>
#include "Texture2D.h"

class EntityPlanetarySystem : public QuEntity
{
public:
	EntityPlanetarySystem(float parentRadius, float childDistance, int numberofChild, Texture2D* texture);

	virtual void OnSpawn(QuWorld* world) override;

	virtual void OnUpdate(const Timer& timer) override;


private:

	DirectX::XMVECTOR m_axisRotation;
	float m_parentRadius;
	float m_childRadius;
	float m_childDistance;
	int m_numberofChild;
	Texture2D* m_texture;
};
