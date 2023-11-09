#pragma once
#include "QuEntity.h"

class Shooting : public QuEntity
{
public:
	Shooting();
	void DetectShooting(DirectX::FXMVECTOR addedVelocity);

	void EnemyShooting(DirectX::FXMVECTOR addedVelocity);

	bool alreadyShooting = false;


private:

	float m_coolDown = 0.2f;
	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
	void InstantiateBullet(DirectX::FXMVECTOR addedVelocity);
	void CoolDown(float dt);

};