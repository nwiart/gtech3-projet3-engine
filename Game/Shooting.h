#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include "EntityController.h"
#include <vector>
#include "Bullet.h"

class Shooting : public QuEntity
{
public:
	Shooting();

	void Shoot();
	Bullet* bullet = new Bullet;
	EntityController* entityController = new EntityController;

private:

	void OnUpdate(Timer timer) override;
	void OnSpawn() override;
	void InstantiateBullet();

	void CoolDown(float dt);

	float m_coolDown = 0.5f;

};