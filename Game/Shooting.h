#pragma once
#include "QuEntity.h"

class Shooting : public QuEntity
{
public:
	Shooting();
	void DetectShooting();

private:

	void OnUpdate(Timer timer) override;
	void OnSpawn() override;
	void InstantiateBullet();
	void CoolDown(float dt);

	float m_coolDown = 0.5f;
	bool alreadyShooting = false;
};