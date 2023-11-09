#pragma once
#include "QuEntity.h"

class Shooting : public QuEntity
{
public:
	Shooting();
	void DetectShooting();

	void EnemyShooting();

	bool alreadyShooting = false;


private:

	float m_coolDown = 0.2f;
	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void InstantiateBullet();
	void CoolDown(float dt);

};