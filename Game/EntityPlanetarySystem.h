#pragma once

#include <QuEntity.h>



class EntityPlanetarySystem : public QuEntity
{
public:
	EntityPlanetarySystem(float parentRadius, float childDistance, int numberofChild);

	virtual void OnSpawn(QuWorld* world) override;

	virtual void OnUpdate(const Timer& timer) override;


private:

	DirectX::XMVECTOR m_axisRotation;

	float m_parentRadius;
	float m_childRadius;
	float m_childDistance;
	int m_numberofChild;
};
