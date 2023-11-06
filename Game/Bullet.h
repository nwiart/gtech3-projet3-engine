#pragma once
#include "QuEntity.h"
#include <vector>
#include "QuEntityRenderModel.h"
#include "QuEntityPhysicsTriggerSphere.h"

class Bullet : public QuEntityPhysicsTriggerSphere
{
public:
	Bullet(float radius);


	void Shoot();

private:
	void MoveBullet(float dt);
	void DestroyBullet();
	bool alreadyShooting = false;
	std::vector<QuEntityRenderModel*> m_bullets;
	void OnUpdate(const Timer& timer) override;
	void OnSpawn(QuWorld* world) override;
};
