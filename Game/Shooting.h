#pragma once
#include "QuEntity.h"
#include "QuEntityRenderModel.h"
#include "EntityController.h"
#include <vector>

class Shooting : public QuEntity
{
	void Shoot();
	void OnUpdate(Timer timer) override;
	void OnSpawn() override;
	void InstantiateBullet();
	void MoveBullet();

	void DestroyBullet();
	void CoolDown(float dt);

private:

	float m_coolDown = 0.f;
	bool alreadyShooting = false;

	DirectX::XMFLOAT4 m_bulletPosition;
	std::vector<QuEntityRenderModel*> m_bullets;
	QuEntityRenderModel* sphereEntity;
	EntityController* m_entityController;
};

