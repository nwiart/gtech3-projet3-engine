#pragma once
#include "QuEntity.h"
#include <vector>
#include "QuEntityRenderModel.h"

class Bullet : public QuEntity
{
public:
	void MoveBullet(float dt);
	void Shoot();
	void DestroyBullet();
	bool alreadyShooting = false;

private:
	std::vector<QuEntityRenderModel*> m_bullets;
	QuEntityRenderModel* sphereEntity;
	void OnUpdate(Timer timer) override;
	void OnSpawn() override;
};
