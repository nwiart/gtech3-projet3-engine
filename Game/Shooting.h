#pragma once
#include "QuEntity.h"

class Shooting : public QuEntity
{
public:
	Shooting();
	void DetectShooting();

private:

	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void InstantiateBullet();
	void CoolDown(float dt);

	float m_coolDown = 0.5f;
	bool alreadyShooting = false;
};